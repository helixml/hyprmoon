#define GST_USE_UNSTABLE_API  // Suppress GStreamer WebRTC warnings

#include "WebRTCManager.hpp"

// Conditional includes for Hyprland integration
#ifdef HYPRLAND_INTEGRATION
#include "../../debug/Log.hpp"
#include "../../Compositor.hpp"
#include "../../managers/input/InputManager.hpp"
// Use Hyprland's Debug namespace directly - no using statements to avoid conflicts
#else
// Standalone debug logging for testing
#include <iostream>
namespace Debug {
    enum LogLevel { LOG, WARN, ERR };
    template<typename... Args>
    void log(LogLevel level, const std::string& format, Args&&... args) {
        const char* prefix = (level == ERR) ? "[ERROR] " : (level == WARN) ? "[WARN] " : "[INFO] ";
        std::cout << prefix << format << std::endl;
    }
}
// using statements removed to avoid conflicts with Hyprland's Debug namespace
#endif

#include <gst/sdp/sdp.h>
#include <gst/app/gstappsink.h>
#include <jsoncpp/json/json.h>
#include <random>
#include <chrono>

CWebRTCManager::CWebRTCManager() {
    Debug::log(Debug::LOG, "WebRTCManager: Creating enhanced WebRTC manager with full bidirectional support");
}

CWebRTCManager::~CWebRTCManager() {
    shutdown();
}

bool CWebRTCManager::initialize() {
    if (m_initialized.load()) {
        Debug::log(Debug::WARN, "WebRTCManager: Already initialized");
        return true;
    }
    
    Debug::log(Debug::LOG, "WebRTCManager: Initializing WebRTC components");
    
    // Initialize GStreamer if not already done
    if (!gst_is_initialized()) {
        gst_init(nullptr, nullptr);
    }
    
    if (!setupBasePipeline()) {
        Debug::log(Debug::ERR, "WebRTCManager: Failed to setup base pipeline");
        return false;
    }
    
    m_initialized.store(true);
    Debug::log(Debug::LOG, "WebRTCManager: Successfully initialized with features:");
    Debug::log(Debug::LOG, "  - Video output: ✓");
    Debug::log(Debug::LOG, "  - Audio input: {}", m_config.enable_audio_input ? "✓" : "✗");
    Debug::log(Debug::LOG, "  - Audio output: {}", m_config.enable_audio_output ? "✓" : "✗");
    Debug::log(Debug::LOG, "  - Keyboard input: {}", m_config.enable_keyboard_input ? "✓" : "✗");
    Debug::log(Debug::LOG, "  - Mouse input: {}", m_config.enable_mouse_input ? "✓" : "✗");
    
    return true;
}

void CWebRTCManager::shutdown() {
    if (!m_initialized.load()) return;
    
    Debug::log(Debug::LOG, "WebRTCManager: Shutting down");
    
    // Close all sessions
    std::lock_guard<std::mutex> lock(m_sessions_mutex);
    for (auto& [clientId, session] : m_sessions) {
        destroySession(clientId);
    }
    m_sessions.clear();
    
    cleanupPipeline();
    m_initialized.store(false);
}

bool CWebRTCManager::setupBasePipeline() {
    m_pipeline = gst_pipeline_new("webrtc-main-pipeline");
    if (!m_pipeline) {
        Debug::log(Debug::ERR, "WebRTCManager: Failed to create main pipeline");
        return false;
    }
    
    Debug::log(Debug::LOG, "WebRTCManager: Base pipeline created successfully");
    return true;
}

bool CWebRTCManager::connectToFrameSource(GstElement* tee_element) {
    if (!m_initialized.load() || !tee_element) {
        Debug::log(Debug::ERR, "WebRTCManager: Invalid state or tee element");
        return false;
    }
    
    Debug::log(Debug::LOG, "WebRTCManager: Connecting to shared frame source");
    
    m_source_tee = tee_element;
    m_connected_to_source.store(true);
    
    return true;
}

std::string CWebRTCManager::createOffer(const std::string& clientId) {
    std::lock_guard<std::mutex> lock(m_sessions_mutex);
    
    // Check if session already exists
    if (m_sessions.find(clientId) != m_sessions.end()) {
        Debug::log(Debug::WARN, "WebRTCManager: Session already exists for client {}", clientId);
        return "";
    }
    
    Debug::log(Debug::LOG, "WebRTCManager: Creating offer for client {}", clientId);
    
    auto session = createSession(clientId);
    if (!session) {
        Debug::log(Debug::ERR, "WebRTCManager: Failed to create session for {}", clientId);
        return "";
    }
    
    if (!linkSessionToPipeline(session.get())) {
        Debug::log(Debug::ERR, "WebRTCManager: Failed to link session to pipeline");
        destroySession(clientId);
        return "";
    }
    
    m_sessions[clientId] = std::move(session);
    
    // The offer will be created automatically via onNegotiationNeeded callback
    return "pending";
}

std::unique_ptr<CWebRTCManager::WebRTCSession> CWebRTCManager::createSession(const std::string& clientId) {
    auto session = std::make_unique<WebRTCSession>();
    session->client_id = clientId;
    session->created_time = std::chrono::steady_clock::now();
    
    // Create webrtcbin
    session->webrtcbin = gst_element_factory_make("webrtcbin", (clientId + "-webrtcbin").c_str());
    if (!session->webrtcbin) {
        Debug::log(Debug::ERR, "WebRTCManager: Failed to create webrtcbin for {}", clientId);
        return nullptr;
    }
    
    // Configure webrtcbin
    g_object_set(session->webrtcbin,
        "bundle-policy", GST_WEBRTC_BUNDLE_POLICY_MAX_BUNDLE,
        "stun-server", m_config.stun_server.c_str(),
        NULL);
    
    // Add TURN servers if configured
    for (const auto& turn_server : m_config.turn_servers) {
        gboolean ret;
        g_signal_emit_by_name(session->webrtcbin, "add-turn-server", turn_server.c_str(), &ret);
    }
    
    // Store callback data for this session
    storeCallbackData(session->webrtcbin, clientId);
    
    // Connect webrtcbin signals
    g_signal_connect(session->webrtcbin, "on-negotiation-needed",
                     G_CALLBACK(on_negotiation_needed_static), this);
    g_signal_connect(session->webrtcbin, "on-ice-candidate",
                     G_CALLBACK(on_ice_candidate_static), this);
    g_signal_connect(session->webrtcbin, "notify::connection-state",
                     G_CALLBACK(on_connection_state_changed_static), this);
    g_signal_connect(session->webrtcbin, "on-data-channel",
                     G_CALLBACK(on_data_channel_created_static), this);
    
    // Create data channel for input if enabled
    if (m_config.enable_keyboard_input || m_config.enable_mouse_input) {
        GstWebRTCDataChannel* data_channel = nullptr;
        GstStructure* options = gst_structure_new("application/data-channel",
            "ordered", G_TYPE_BOOLEAN, TRUE,
            "protocol", G_TYPE_STRING, "input-protocol",
            NULL);
        
        g_signal_emit_by_name(session->webrtcbin, "create-data-channel", 
                             "input", options, &data_channel);
        
        if (data_channel) {
            // Connect data channel signals
            g_signal_connect(data_channel, "on-open", 
                           G_CALLBACK(on_data_channel_open_static), this);
            g_signal_connect(data_channel, "on-message-data",
                           G_CALLBACK(on_data_channel_message_static), this);
            g_signal_connect(data_channel, "on-close",
                           G_CALLBACK(on_data_channel_close_static), this);
            
            storeCallbackData(data_channel, clientId);
            session->input_channel_id = 0; // First data channel
        }
        
        gst_structure_free(options);
    }
    
    Debug::log(Debug::LOG, "WebRTCManager: Created session for client {}", clientId);
    return session;
}

bool CWebRTCManager::linkSessionToPipeline(WebRTCSession* session) {
    if (!session || !m_connected_to_source.load()) {
        return false;
    }
    
    Debug::log(Debug::LOG, "WebRTCManager: Linking session {} to pipeline", session->client_id);
    
    // Get the main pipeline (where the tee is located)
    GstElement* main_pipeline = GST_ELEMENT(gst_element_get_parent(m_source_tee));
    if (!main_pipeline) {
        Debug::log(Debug::ERR, "WebRTCManager: Could not find main pipeline");
        return false;
    }
    
    // Create video pipeline elements
    session->video_queue = gst_element_factory_make("queue", 
        (session->client_id + "-video-queue").c_str());
    session->videoconvert = gst_element_factory_make("videoconvert", 
        (session->client_id + "-videoconvert").c_str());
    
    if (!session->video_queue || !session->videoconvert) {
        Debug::log(Debug::ERR, "WebRTCManager: Failed to create video pipeline elements");
        return false;
    }
    
    // Add elements to main pipeline
    gst_bin_add_many(GST_BIN(main_pipeline), 
                     session->video_queue, 
                     session->videoconvert,
                     session->webrtcbin, 
                     NULL);
    
    // Link video pipeline: tee → queue → videoconvert → webrtcbin
    GstPad* tee_pad = gst_element_request_pad_simple(m_source_tee, "src_%u");
    GstPad* queue_sink = gst_element_get_static_pad(session->video_queue, "sink");
    
    if (gst_pad_link(tee_pad, queue_sink) != GST_PAD_LINK_OK) {
        Debug::log(Debug::ERR, "WebRTCManager: Failed to link tee to queue");
        gst_object_unref(tee_pad);
        gst_object_unref(queue_sink);
        return false;
    }
    
    if (!gst_element_link_many(session->video_queue, session->videoconvert, NULL)) {
        Debug::log(Debug::ERR, "WebRTCManager: Failed to link video pipeline");
        gst_object_unref(tee_pad);
        gst_object_unref(queue_sink);
        return false;
    }
    
    // Link videoconvert to webrtcbin
    GstPad* convert_src = gst_element_get_static_pad(session->videoconvert, "src");
    GstPad* webrtc_sink = gst_element_request_pad_simple(session->webrtcbin, "sink_%u");
    
    if (gst_pad_link(convert_src, webrtc_sink) != GST_PAD_LINK_OK) {
        Debug::log(Debug::ERR, "WebRTCManager: Failed to link to webrtcbin");
        gst_object_unref(tee_pad);
        gst_object_unref(queue_sink);
        gst_object_unref(convert_src);
        gst_object_unref(webrtc_sink);
        return false;
    }
    
    gst_object_unref(tee_pad);
    gst_object_unref(queue_sink);
    gst_object_unref(convert_src);
    gst_object_unref(webrtc_sink);
    
    // Setup audio pipeline if enabled
    if (m_config.enable_audio_output || m_config.enable_audio_input) {
        setupAudioPipeline(session, main_pipeline);
    }
    
    // Sync state with main pipeline
    gst_element_sync_state_with_parent(session->video_queue);
    gst_element_sync_state_with_parent(session->videoconvert);
    gst_element_sync_state_with_parent(session->webrtcbin);
    
    Debug::log(Debug::LOG, "WebRTCManager: Successfully linked session to pipeline");
    return true;
}

bool CWebRTCManager::setupAudioPipeline(WebRTCSession* session, GstElement* main_pipeline) {
    // Audio output (desktop → browser)
    if (m_config.enable_audio_output) {
        session->audio_src = gst_element_factory_make("pulsesrc", 
            (session->client_id + "-audio-src").c_str());
        session->audio_queue_out = gst_element_factory_make("queue", 
            (session->client_id + "-audio-queue-out").c_str());
        
        if (session->audio_src && session->audio_queue_out) {
            // Configure audio source
            g_object_set(session->audio_src,
                "device", "default.monitor",  // Capture desktop audio
                NULL);
            
            gst_bin_add_many(GST_BIN(main_pipeline), 
                           session->audio_src, 
                           session->audio_queue_out, 
                           NULL);
            
            // Link audio output: pulsesrc → queue → webrtcbin
            if (gst_element_link_many(session->audio_src, session->audio_queue_out, NULL)) {
                GstPad* audio_src = gst_element_get_static_pad(session->audio_queue_out, "src");
                GstPad* webrtc_audio_sink = gst_element_request_pad_simple(session->webrtcbin, "sink_%u");
                
                if (gst_pad_link(audio_src, webrtc_audio_sink) == GST_PAD_LINK_OK) {
                    gst_element_sync_state_with_parent(session->audio_src);
                    gst_element_sync_state_with_parent(session->audio_queue_out);
                    Debug::log(Debug::LOG, "WebRTCManager: Audio output pipeline linked");
                }
                
                gst_object_unref(audio_src);
                gst_object_unref(webrtc_audio_sink);
            }
        }
    }
    
    // Audio input (browser → speakers) - setup sink for received audio
    if (m_config.enable_audio_input) {
        session->audio_sink = gst_element_factory_make("pulsesink", 
            (session->client_id + "-audio-sink").c_str());
        session->audio_queue_in = gst_element_factory_make("queue", 
            (session->client_id + "-audio-queue-in").c_str());
        
        if (session->audio_sink && session->audio_queue_in) {
            gst_bin_add_many(GST_BIN(main_pipeline), 
                           session->audio_queue_in,
                           session->audio_sink, 
                           NULL);
            
            // This will be connected when webrtcbin creates src pads
            gst_element_sync_state_with_parent(session->audio_queue_in);
            gst_element_sync_state_with_parent(session->audio_sink);
        }
    }
    
    return true;
}

// Callback implementations
void CWebRTCManager::onNegotiationNeeded(GstElement* webrtcbin, const std::string& clientId) {
    Debug::log(Debug::LOG, "WebRTCManager: Negotiation needed for client {}", clientId);
    
    GstPromise* promise = gst_promise_new_with_change_func(on_offer_created_static, this, nullptr);
    g_signal_emit_by_name(webrtcbin, "create-offer", nullptr, promise);
}

void CWebRTCManager::onOfferCreatedCallback(GstPromise* promise, const std::string& clientId) {
    const GstStructure* reply = gst_promise_get_reply(promise);
    GstWebRTCSessionDescription* offer = nullptr;
    
    gst_structure_get(reply, "offer", GST_TYPE_WEBRTC_SESSION_DESCRIPTION, &offer, nullptr);
    gst_promise_unref(promise);
    
    if (!offer) {
        Debug::log(Debug::ERR, "WebRTCManager: Failed to create offer for {}", clientId);
        return;
    }
    
    // Set local description
    GstPromise* local_promise = gst_promise_new();
    auto session = getSession(clientId);
    if (session) {
        g_signal_emit_by_name(session->webrtcbin, "set-local-description", offer, local_promise);
        gst_promise_interrupt(local_promise);
        gst_promise_unref(local_promise);
        
        session->offer_created = true;
    }
    
    // Send offer via callback
    if (onOfferCreated) {
        gchar* sdp_string = gst_sdp_message_as_text(offer->sdp);
        onOfferCreated(clientId, std::string(sdp_string));
        g_free(sdp_string);
    }
    
    gst_webrtc_session_description_free(offer);
    Debug::log(Debug::LOG, "WebRTCManager: Offer created and sent for client {}", clientId);
}

void CWebRTCManager::onICECandidateCallback(GstElement* webrtcbin, guint mlineindex, 
                                           const gchar* candidate, const std::string& clientId) {
    if (onICECandidate) {
        // Create JSON for ICE candidate
        Json::Value ice_json;
        ice_json["candidate"] = candidate;
        ice_json["sdpMLineIndex"] = mlineindex;
        ice_json["sdpMid"] = "";
        
        Json::StreamWriterBuilder builder;
        std::string ice_string = Json::writeString(builder, ice_json);
        onICECandidate(clientId, ice_string);
    }
}

void CWebRTCManager::onDataChannelOpen(GstWebRTCDataChannel* channel, const std::string& clientId) {
    Debug::log(Debug::LOG, "WebRTCManager: Data channel opened for client {}", clientId);
    
    auto session = getSession(clientId);
    if (session) {
        session->input_channel_ready = true;
        session->permissions_granted = true;  // Assume permissions granted when channel opens
    }
}

void CWebRTCManager::onDataChannelMessage(GstWebRTCDataChannel* channel, GBytes* data, const std::string& clientId) {
    gsize size;
    const gchar* message_data = (const gchar*)g_bytes_get_data(data, &size);
    std::string message(message_data, size);
    
    processInputMessage(clientId, message);
}

void CWebRTCManager::processInputMessage(const std::string& clientId, const std::string& message) {
    try {
        Json::Value root;
        Json::CharReaderBuilder builder;
        std::string errors;
        std::istringstream stream(message);
        
        if (!Json::parseFromStream(builder, stream, &root, &errors)) {
            Debug::log(Debug::WARN, "WebRTCManager: Invalid JSON input from {}: {}", clientId, errors);
            return;
        }
        
        std::string type = root.get("type", "").asString();
        
        if (type == "keyboard" && m_config.enable_keyboard_input) {
            int keycode = root.get("keycode", 0).asInt();
            bool pressed = root.get("pressed", false).asBool();
            uint32_t modifiers = root.get("modifiers", 0).asUInt();
            processKeyboardInput(clientId, keycode, pressed, modifiers);
            
        } else if (type == "mouse_button" && m_config.enable_mouse_input) {
            double x = root.get("x", 0.0).asDouble();
            double y = root.get("y", 0.0).asDouble();
            int button = root.get("button", 0).asInt();
            bool pressed = root.get("pressed", false).asBool();
            processMouseInput(clientId, x, y, button, pressed);
            
        } else if (type == "mouse_move" && m_config.enable_mouse_input) {
            double x = root.get("x", 0.0).asDouble();
            double y = root.get("y", 0.0).asDouble();
            processMouseMove(clientId, x, y);
            
        } else if (type == "mouse_scroll" && m_config.enable_mouse_input) {
            double delta_x = root.get("deltaX", 0.0).asDouble();
            double delta_y = root.get("deltaY", 0.0).asDouble();
            processMouseScroll(clientId, delta_x, delta_y);
            
        } else if (type == "touch" && m_config.enable_mouse_input) {
            double x = root.get("x", 0.0).asDouble();
            double y = root.get("y", 0.0).asDouble();
            bool pressed = root.get("pressed", false).asBool();
            int touch_id = root.get("id", 0).asInt();
            processTouchInput(clientId, x, y, pressed, touch_id);
        }
        
    } catch (const std::exception& e) {
        Debug::log(Debug::ERR, "WebRTCManager: Error processing input from {}: {}", clientId, e.what());
    }
}

void CWebRTCManager::processKeyboardInput(const std::string& clientId, int keycode, bool pressed, uint32_t modifiers) {
    if (onKeyboardInput) {
        onKeyboardInput(keycode, pressed, modifiers);
    }
    
    // Also integrate directly with Hyprland input system
#ifdef HYPRLAND_INTEGRATION
    if (g_pInputManager) {
        // Convert web keycode to Linux keycode and inject
        // This would need proper keycode mapping
        Debug::log(Debug::LOG, "WebRTCManager: Keyboard input from {}: key={}, pressed={}, mods={}", 
                  clientId, keycode, pressed, modifiers);
    }
#else
    // Standalone mode - would need uinput integration
    Debug::log(Debug::LOG, "WebRTCManager: Standalone keyboard input: key={}, pressed={}, mods={}", 
              keycode, pressed, modifiers);
#endif
}

void CWebRTCManager::processMouseInput(const std::string& clientId, double x, double y, int button, bool pressed) {
    if (onMouseInput) {
        onMouseInput(x, y, button, pressed);
    }
    
    Debug::log(Debug::LOG, "WebRTCManager: Mouse input from {}: pos=({:.2f},{:.2f}), button={}, pressed={}", 
              clientId, x, y, button, pressed);
}

void CWebRTCManager::processMouseMove(const std::string& clientId, double x, double y) {
    auto session = getSession(clientId);
    if (session) {
        session->input_state.last_mouse_x = x;
        session->input_state.last_mouse_y = y;
    }
    
    if (onMouseMove) {
        onMouseMove(x, y);
    }
}

void CWebRTCManager::processMouseScroll(const std::string& clientId, double delta_x, double delta_y) {
    if (onMouseScroll) {
        onMouseScroll(delta_x, delta_y);
    }
    
    Debug::log(Debug::LOG, "WebRTCManager: Mouse scroll from {}: delta=({:.2f},{:.2f})", 
              clientId, delta_x, delta_y);
}

void CWebRTCManager::processTouchInput(const std::string& clientId, double x, double y, bool pressed, int touch_id) {
    // Convert touch to mouse for now
    if (pressed) {
        processMouseInput(clientId, x, y, 1, true);  // Left click
    } else {
        processMouseInput(clientId, x, y, 1, false);
    }
}

// Static callback implementations
void CWebRTCManager::on_negotiation_needed_static(GstElement* element, gpointer user_data) {
    CWebRTCManager* self = static_cast<CWebRTCManager*>(user_data);
    std::string clientId = self->getCallbackClientId(element);
    if (!clientId.empty()) {
        self->onNegotiationNeeded(element, clientId);
    }
}

void CWebRTCManager::on_offer_created_static(GstPromise* promise, gpointer user_data) {
    CWebRTCManager* self = static_cast<CWebRTCManager*>(user_data);
    (void)self; // Suppress unused variable warning
    // Need to find which client this promise belongs to
    // For simplicity, we'll handle this in the session context
    // In practice, you'd need better promise tracking
}

void CWebRTCManager::on_ice_candidate_static(GstElement* webrtc, guint mlineindex, gchar* candidate, gpointer user_data) {
    CWebRTCManager* self = static_cast<CWebRTCManager*>(user_data);
    std::string clientId = self->getCallbackClientId(webrtc);
    if (!clientId.empty()) {
        self->onICECandidateCallback(webrtc, mlineindex, candidate, clientId);
    }
}

void CWebRTCManager::on_data_channel_open_static(GstWebRTCDataChannel* channel, gpointer user_data) {
    CWebRTCManager* self = static_cast<CWebRTCManager*>(user_data);
    std::string clientId = self->getCallbackClientId(channel);
    if (!clientId.empty()) {
        self->onDataChannelOpen(channel, clientId);
    }
}

void CWebRTCManager::on_data_channel_message_static(GstWebRTCDataChannel* channel, GBytes* data, gpointer user_data) {
    CWebRTCManager* self = static_cast<CWebRTCManager*>(user_data);
    std::string clientId = self->getCallbackClientId(channel);
    if (!clientId.empty()) {
        self->onDataChannelMessage(channel, data, clientId);
    }
}

// Utility methods
std::string CWebRTCManager::generateSessionId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    
    std::string id;
    for (int i = 0; i < 32; ++i) {
        id += "0123456789abcdef"[dis(gen)];
    }
    return id;
}

void CWebRTCManager::storeCallbackData(gpointer key, const std::string& clientId) {
    std::lock_guard<std::mutex> lock(m_callback_mutex);
    m_callback_data[key] = std::make_unique<CallbackData>();
    m_callback_data[key]->manager = this;
    m_callback_data[key]->client_id = clientId;
}

std::string CWebRTCManager::getCallbackClientId(gpointer key) {
    std::lock_guard<std::mutex> lock(m_callback_mutex);
    auto it = m_callback_data.find(key);
    return (it != m_callback_data.end()) ? it->second->client_id : "";
}

void CWebRTCManager::removeCallbackData(gpointer key) {
    std::lock_guard<std::mutex> lock(m_callback_mutex);
    m_callback_data.erase(key);
}

CWebRTCManager::WebRTCSession* CWebRTCManager::getSession(const std::string& clientId) {
    auto it = m_sessions.find(clientId);
    return (it != m_sessions.end()) ? it->second.get() : nullptr;
}

void CWebRTCManager::destroySession(const std::string& clientId) {
    Debug::log(Debug::LOG, "WebRTCManager: Destroying session for client {}", clientId);
    
    auto it = m_sessions.find(clientId);
    if (it != m_sessions.end()) {
        auto& session = it->second;
        
        // Remove callback data
        if (session->webrtcbin) {
            removeCallbackData(session->webrtcbin);
        }
        
        // Cleanup will happen in destructor
        m_sessions.erase(it);
    }
}

void CWebRTCManager::cleanupPipeline() {
    if (m_pipeline) {
        gst_element_set_state(m_pipeline, GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(m_pipeline));
        m_pipeline = nullptr;
    }
}

bool CWebRTCManager::setRemoteAnswer(const std::string& clientId, const std::string& sdp) {
    auto session = getSession(clientId);
    if (!session) {
        Debug::log(Debug::ERR, "WebRTCManager: No session found for client {}", clientId);
        return false;
    }
    
    GstWebRTCSessionDescription* answer = createSessionDescription(sdp, GST_WEBRTC_SDP_TYPE_ANSWER);
    if (!answer) {
        Debug::log(Debug::ERR, "WebRTCManager: Failed to parse SDP answer");
        return false;
    }
    
    GstPromise* promise = gst_promise_new();
    g_signal_emit_by_name(session->webrtcbin, "set-remote-description", answer, promise);
    gst_promise_interrupt(promise);
    gst_promise_unref(promise);
    gst_webrtc_session_description_free(answer);
    
    Debug::log(Debug::LOG, "WebRTCManager: Set remote answer for client {}", clientId);
    return true;
}

GstWebRTCSessionDescription* CWebRTCManager::createSessionDescription(const std::string& sdp, GstWebRTCSDPType type) {
    GstSDPMessage* sdp_msg;
    if (gst_sdp_message_new(&sdp_msg) != GST_SDP_OK) {
        return nullptr;
    }
    
    if (gst_sdp_message_parse_buffer((guint8*)sdp.c_str(), sdp.length(), sdp_msg) != GST_SDP_OK) {
        gst_sdp_message_free(sdp_msg);
        return nullptr;
    }
    
    return gst_webrtc_session_description_new(type, sdp_msg);
}

size_t CWebRTCManager::getClientCount() const {
    std::lock_guard<std::mutex> lock(m_sessions_mutex);
    return m_sessions.size();
}

std::vector<std::string> CWebRTCManager::getActiveClients() const {
    std::lock_guard<std::mutex> lock(m_sessions_mutex);
    std::vector<std::string> clients;
    for (const auto& [clientId, session] : m_sessions) {
        if (session->connected) {
            clients.push_back(clientId);
        }
    }
    return clients;
}

bool CWebRTCManager::addICECandidate(const std::string& clientId, const std::string& candidate) {
    std::lock_guard<std::mutex> lock(m_sessions_mutex);
    auto it = m_sessions.find(clientId);
    if (it == m_sessions.end()) {
        Debug::log(Debug::ERR, "WebRTCManager: No session found for client {}", clientId);
        return false;
    }
    
    auto& session = it->second;
    if (!session->webrtcbin) {
        Debug::log(Debug::ERR, "WebRTCManager: No webrtcbin for client {}", clientId);
        return false;
    }
    
    // Parse ICE candidate and add to webrtcbin
    g_signal_emit_by_name(session->webrtcbin, "add-ice-candidate", 0, candidate.c_str());
    Debug::log(Debug::LOG, "WebRTCManager: Added ICE candidate for client {}", clientId);
    return true;
}

void CWebRTCManager::closeSession(const std::string& clientId) {
    std::lock_guard<std::mutex> lock(m_sessions_mutex);
    auto it = m_sessions.find(clientId);
    if (it == m_sessions.end()) {
        Debug::log(Debug::WARN, "WebRTCManager: No session found for client {}", clientId);
        return;
    }
    
    auto& session = it->second;
    if (session->webrtcbin) {
        gst_element_set_state(session->webrtcbin, GST_STATE_NULL);
        gst_object_unref(session->webrtcbin);
        session->webrtcbin = nullptr;
    }
    
    // Clean up other pipeline elements
    if (session->video_queue) {
        gst_element_set_state(session->video_queue, GST_STATE_NULL);
        gst_object_unref(session->video_queue);
        session->video_queue = nullptr;
    }
    
    if (session->videoconvert) {
        gst_element_set_state(session->videoconvert, GST_STATE_NULL);
        gst_object_unref(session->videoconvert);
        session->videoconvert = nullptr;
    }
    
    m_sessions.erase(it);
    Debug::log(Debug::LOG, "WebRTCManager: Closed session for client {}", clientId);
}

// Missing static callback implementations
void CWebRTCManager::on_connection_state_changed_static(GstElement* webrtcbin, GParamSpec* pspec, gpointer user_data) {
    CWebRTCManager* self = static_cast<CWebRTCManager*>(user_data);
    std::string clientId = self->getCallbackClientId(webrtcbin);
    if (!clientId.empty()) {
        GstWebRTCPeerConnectionState state;
        g_object_get(webrtcbin, "connection-state", &state, NULL);
        bool connected = (state == GST_WEBRTC_PEER_CONNECTION_STATE_CONNECTED);
        
        // Update session state
        {
            std::lock_guard<std::mutex> lock(self->m_sessions_mutex);
            auto it = self->m_sessions.find(clientId);
            if (it != self->m_sessions.end()) {
                it->second->connected = connected;
            }
        }
        
        if (self->onConnectionStateChanged) {
            self->onConnectionStateChanged(clientId, connected);
        }
        Debug::log(Debug::LOG, "WebRTCManager: Client {} connection state: {}", clientId, connected ? "connected" : "disconnected");
    }
}

void CWebRTCManager::on_data_channel_created_static(GstElement* webrtcbin, GstWebRTCDataChannel* channel, gpointer user_data) {
    CWebRTCManager* self = static_cast<CWebRTCManager*>(user_data);
    std::string clientId = self->getCallbackClientId(webrtcbin);
    if (!clientId.empty()) {
        Debug::log(Debug::LOG, "WebRTCManager: Data channel created for client {}", clientId);
        
        // Connect data channel callbacks
        g_signal_connect(channel, "on-open", G_CALLBACK(CWebRTCManager::on_data_channel_open_static), user_data);
        g_signal_connect(channel, "on-close", G_CALLBACK(CWebRTCManager::on_data_channel_close_static), user_data);
        g_signal_connect(channel, "on-message-data", G_CALLBACK(CWebRTCManager::on_data_channel_message_static), user_data);
    }
}

void CWebRTCManager::on_data_channel_close_static(GstWebRTCDataChannel* channel, gpointer user_data) {
    CWebRTCManager* self = static_cast<CWebRTCManager*>(user_data);
    std::string clientId = self->getCallbackClientId(channel);
    if (!clientId.empty()) {
        Debug::log(Debug::LOG, "WebRTCManager: Data channel closed for client {}", clientId);
    }
}