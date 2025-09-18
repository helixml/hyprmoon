#include "WolfMoonlightServer.hpp"
#include "../../debug/Log.hpp"
#include <sstream>
#include <iomanip>
#include <random>
#include <cstring>
#include <gst/allocators/gstdmabuf.h>
#include <gst/video/video.h>

// Wolf REST server includes - only declarations to avoid linker conflicts
#include "../rest/rest.hpp"

namespace wolf {
namespace core {

// MoonlightState implementation
MoonlightState::MoonlightState(const MoonlightConfig& config) : config_(config) {
}

MoonlightState::~MoonlightState() {
    shutdown();
}

bool MoonlightState::initialize() {
    Debug::log(LOG, "WolfMoonlightState: Initializing with UUID: {}", config_.uuid);
    return true;
}

void MoonlightState::shutdown() {
    std::lock_guard<std::mutex> lock(mutex_);
    active_sessions_.clear();
}

void MoonlightState::addPairedClient(const HyprlandPairedClient& client) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Remove any existing client with same ID
    paired_clients_.erase(
        std::remove_if(paired_clients_.begin(), paired_clients_.end(),
            [&client](const HyprlandPairedClient& c) { return c.client_id == client.client_id; }),
        paired_clients_.end());

    paired_clients_.push_back(client);
    Debug::log(LOG, "WolfMoonlightState: Paired client {} ({})", client.client_name, client.client_id);
}

void MoonlightState::removePairedClient(const std::string& client_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    paired_clients_.erase(
        std::remove_if(paired_clients_.begin(), paired_clients_.end(),
            [&client_id](const HyprlandPairedClient& c) { return c.client_id == client_id; }),
        paired_clients_.end());
    
    Debug::log(LOG, "WolfMoonlightState: Unpaired client {}", client_id);
}

std::vector<HyprlandPairedClient> MoonlightState::getPairedClients() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return paired_clients_;
}

std::shared_ptr<StreamSession> MoonlightState::createSession(const HyprlandPairedClient& client, const std::string& client_ip) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Generate session ID
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;
    
    std::stringstream ss;
    ss << std::hex << dis(gen);
    std::string session_id = ss.str();
    
    auto session = std::make_shared<StreamSession>();
    session->session_id = session_id;
    session->client = client;
    session->client_ip = client_ip;
    session->video_port = config_.video_port;
    session->audio_port = config_.audio_port;
    session->start_time = std::chrono::system_clock::now();
    
    active_sessions_[session_id] = session;
    
    Debug::log(LOG, "WolfMoonlightState: Created session {} for client {}", session_id, client.client_name);
    return session;
}

void MoonlightState::destroySession(const std::string& session_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    active_sessions_.erase(session_id);
    Debug::log(LOG, "WolfMoonlightState: Destroyed session {}", session_id);
}

std::shared_ptr<StreamSession> MoonlightState::getSession(const std::string& session_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = active_sessions_.find(session_id);
    return (it != active_sessions_.end()) ? it->second : nullptr;
}

// StreamingEngine implementation
StreamingEngine::StreamingEngine(std::shared_ptr<MoonlightState> state) 
    : state_(state), pipeline_(nullptr), app_src_(nullptr), encoder_(nullptr), 
      payloader_(nullptr), sink_(nullptr), running_(false) {
}

StreamingEngine::~StreamingEngine() {
    shutdown();
}

bool StreamingEngine::initialize() {
    Debug::log(LOG, "WolfStreamingEngine: Initializing GStreamer pipeline");
    
    if (!setupGStreamerPipeline()) {
        Debug::log(ERR, "WolfStreamingEngine: Failed to setup GStreamer pipeline");
        return false;
    }
    
    running_ = true;
    gst_thread_ = std::thread(&StreamingEngine::gstreamerThreadMain, this);
    
    Debug::log(LOG, "WolfStreamingEngine: Initialized successfully");
    return true;
}

void StreamingEngine::shutdown() {
    if (running_) {
        running_ = false;
        if (gst_thread_.joinable()) {
            gst_thread_.join();
        }
    }
    
    cleanupGStreamerPipeline();
}

void StreamingEngine::pushFrame(const void* frame_data, size_t size, int width, int height, uint32_t format) {
    if (!running_ || !app_src_) {
        return;
    }

    // Create GStreamer buffer from frame data
    GstBuffer* buffer = gst_buffer_new_allocate(nullptr, size, nullptr);
    if (!buffer) {
        Debug::log(ERR, "WolfStreamingEngine: Failed to allocate GstBuffer");
        return;
    }

    GstMapInfo map;
    if (gst_buffer_map(buffer, &map, GST_MAP_WRITE)) {
        memcpy(map.data, frame_data, size);
        gst_buffer_unmap(buffer, &map);

        // Push buffer to app source
        GstFlowReturn ret = gst_app_src_push_buffer(GST_APP_SRC(app_src_), buffer);
        if (ret != GST_FLOW_OK) {
            Debug::log(WARN, "WolfStreamingEngine: Failed to push buffer: {}", (int)ret);
        }
    } else {
        Debug::log(ERR, "WolfStreamingEngine: Failed to map GstBuffer");
        gst_buffer_unref(buffer);
    }
}

void StreamingEngine::pushFrameDMABuf(int dmabuf_fd, uint32_t stride, uint64_t modifier, int width, int height, uint32_t format, wlr_buffer* buffer_ref) {
    if (!running_ || !app_src_) {
        if (buffer_ref) {
            wlr_buffer_unlock(buffer_ref); // Release buffer if we can't process
        }
        return;
    }

    Debug::log(LOG, "WolfStreamingEngine: Zero-copy DMA-BUF frame - fd: {}, {}x{}, stride: {}, modifier: {}",
              dmabuf_fd, width, height, stride, modifier);

    // Create GStreamer buffer from DMA-BUF file descriptor
    // Use gst_dmabuf_allocator for zero-copy GPU buffer sharing
    GstAllocator* dmabuf_allocator = gst_dmabuf_allocator_new();
    if (!dmabuf_allocator) {
        Debug::log(ERR, "WolfStreamingEngine: Failed to create DMA-BUF allocator");
        if (buffer_ref) {
            wlr_buffer_unlock(buffer_ref); // Release on error
        }
        return;
    }

    // Create memory object wrapping the DMA-BUF
    size_t dmabuf_size = stride * height;
    GstMemory* dmabuf_memory = gst_dmabuf_allocator_alloc(dmabuf_allocator, dmabuf_fd, dmabuf_size);

    if (!dmabuf_memory) {
        Debug::log(ERR, "WolfStreamingEngine: Failed to wrap DMA-BUF fd {} in GstMemory", dmabuf_fd);
        gst_object_unref(dmabuf_allocator);
        if (buffer_ref) {
            wlr_buffer_unlock(buffer_ref); // Release on error
        }
        return;
    }

    // Create GStreamer buffer using the DMA-BUF memory
    GstBuffer* buffer = gst_buffer_new();
    gst_buffer_append_memory(buffer, dmabuf_memory);

    // Add video metadata for proper format handling
    gst_buffer_add_video_meta(buffer, GST_VIDEO_FRAME_FLAG_NONE, GST_VIDEO_FORMAT_BGRx, width, height);

    // Attach buffer reference for cleanup when GStreamer finishes processing
    if (buffer_ref) {
        gst_mini_object_set_qdata(GST_MINI_OBJECT(buffer),
                                 g_quark_from_static_string("wlr_buffer_ref"),
                                 buffer_ref,
                                 (GDestroyNotify)wlr_buffer_unlock);
    }

    // Push zero-copy buffer to app source
    GstFlowReturn ret = gst_app_src_push_buffer(GST_APP_SRC(app_src_), buffer);
    if (ret != GST_FLOW_OK) {
        Debug::log(WARN, "WolfStreamingEngine: Failed to push DMA-BUF buffer: {}", (int)ret);
        // Buffer cleanup will happen automatically via qdata destructor
    } else {
        Debug::log(LOG, "WolfStreamingEngine: Successfully pushed zero-copy DMA-BUF frame");
    }

    gst_object_unref(dmabuf_allocator);
    // Note: wlr_buffer will be unlocked automatically when GStreamer finishes with the buffer
}

bool StreamingEngine::startStreaming(const std::string& session_id) {
    auto session = state_->getSession(session_id);
    if (!session) {
        Debug::log(ERR, "WolfStreamingEngine: Session {} not found", session_id);
        return false;
    }
    
    session->active = true;
    
    if (pipeline_) {
        gst_element_set_state(pipeline_, GST_STATE_PLAYING);
    }
    
    Debug::log(LOG, "WolfStreamingEngine: Started streaming for session {}", session_id);
    return true;
}

void StreamingEngine::stopStreaming(const std::string& session_id) {
    auto session = state_->getSession(session_id);
    if (session) {
        session->active = false;
    }
    
    if (pipeline_) {
        gst_element_set_state(pipeline_, GST_STATE_PAUSED);
    }
    
    Debug::log(LOG, "WolfStreamingEngine: Stopped streaming for session {}", session_id);
}

bool StreamingEngine::isStreaming(const std::string& session_id) const {
    auto session = state_->getSession(session_id);
    return session && session->active;
}

bool StreamingEngine::setupGStreamerPipeline() {
    std::string pipeline_desc = buildPipelineDescription();
    Debug::log(LOG, "WolfStreamingEngine: Creating pipeline: {}", pipeline_desc);
    
    GError* error = nullptr;
    pipeline_ = gst_parse_launch(pipeline_desc.c_str(), &error);
    
    if (!pipeline_) {
        Debug::log(ERR, "WolfStreamingEngine: Pipeline parse error: {}", error ? error->message : "unknown");
        if (error) g_error_free(error);
        return false;
    }
    
    // Get references to key elements
    app_src_ = gst_bin_get_by_name(GST_BIN(pipeline_), "hyprland_src");
    if (!app_src_) {
        Debug::log(ERR, "WolfStreamingEngine: Failed to get app source element");
        return false;
    }
    
    // Configure app source
    g_object_set(app_src_,
        "caps", gst_caps_from_string("video/x-raw,format=BGRA,width=2360,height=1640,framerate=120/1"),
        "format", GST_FORMAT_TIME,
        "is-live", TRUE,
        nullptr);
    
    return true;
}

void StreamingEngine::cleanupGStreamerPipeline() {
    if (pipeline_) {
        gst_element_set_state(pipeline_, GST_STATE_NULL);
        gst_object_unref(pipeline_);
        pipeline_ = nullptr;
    }
    
    app_src_ = nullptr;
    encoder_ = nullptr;
    payloader_ = nullptr;
    sink_ = nullptr;
}

void StreamingEngine::gstreamerThreadMain() {
    Debug::log(LOG, "WolfStreamingEngine: GStreamer thread started");
    
    GMainLoop* loop = g_main_loop_new(nullptr, FALSE);
    
    if (pipeline_) {
        // Add bus watch
        GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline_));
        gst_bus_add_signal_watch(bus);
        
        // Connect to bus signals
        g_signal_connect(bus, "message::error", G_CALLBACK(+[](GstBus*, GstMessage* msg, gpointer) {
            GError* error;
            gchar* debug_info;
            gst_message_parse_error(msg, &error, &debug_info);
            Debug::log(ERR, "WolfStreamingEngine: GStreamer error: {} ({})", 
                      error->message, debug_info ? debug_info : "no debug info");
            g_error_free(error);
            g_free(debug_info);
        }), nullptr);
        
        g_signal_connect(bus, "message::eos", G_CALLBACK(+[](GstBus*, GstMessage*, gpointer loop) {
            Debug::log(LOG, "WolfStreamingEngine: End of stream");
            g_main_loop_quit((GMainLoop*)loop);
        }), loop);
        
        gst_object_unref(bus);
        
        // Set pipeline to ready state
        gst_element_set_state(pipeline_, GST_STATE_READY);
    }
    
    // Run main loop until shutdown
    while (running_) {
        g_main_context_iteration(nullptr, FALSE);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    g_main_loop_unref(loop);
    Debug::log(LOG, "WolfStreamingEngine: GStreamer thread stopped");
}

std::string StreamingEngine::buildPipelineDescription() const {
    const auto& config = state_->getConfig();
    
    // Build a simple pipeline for now
    std::stringstream ss;
    ss << "appsrc name=hyprland_src ! ";
    ss << "videoconvert ! ";
    
    // Choose encoder based on config
    if (config.video.encoder == "nvenc" || config.video.encoder == "auto") {
        ss << "nvh264enc bitrate=" << config.video.bitrate << " ! ";
    } else if (config.video.encoder == "vaapi") {
        ss << "vaapih264enc bitrate=" << config.video.bitrate << " ! ";
    } else if (config.video.encoder == "qsv") {
        ss << "qsvh264enc bitrate=" << config.video.bitrate << " ! ";
    } else {
        ss << "x264enc bitrate=" << config.video.bitrate << " tune=zerolatency ! ";
    }
    
    ss << "h264parse ! ";
    ss << "rtph264pay ! ";
    ss << "udpsink host=127.0.0.1 port=" << config.video_port;
    
    return ss.str();
}

// ControlServer implementation  
ControlServer::ControlServer(std::shared_ptr<MoonlightState> state)
    : state_(state), server_(nullptr), running_(false) {
}

ControlServer::~ControlServer() {
    shutdown();
}

bool ControlServer::initialize() {
    Debug::log(LOG, "WolfControlServer: Initializing ENet server on port {}", state_->getConfig().control_port);
    
    // Initialize ENet
    if (enet_initialize() != 0) {
        Debug::log(ERR, "WolfControlServer: Failed to initialize ENet");
        return false;
    }
    
    // Create server
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = state_->getConfig().control_port;
    
    server_ = enet_host_create(&address, 32, 2, 0, 0);
    if (!server_) {
        Debug::log(ERR, "WolfControlServer: Failed to create ENet server");
        enet_deinitialize();
        return false;
    }
    
    running_ = true;
    server_thread_ = std::thread(&ControlServer::serverThreadMain, this);
    
    Debug::log(LOG, "WolfControlServer: Initialized successfully");
    return true;
}

void ControlServer::shutdown() {
    if (running_) {
        running_ = false;
        if (server_thread_.joinable()) {
            server_thread_.join();
        }
    }
    
    if (server_) {
        enet_host_destroy(server_);
        server_ = nullptr;
        enet_deinitialize();
    }
}

void ControlServer::handleMouseInput(int x, int y, int button, bool pressed) {
    // TODO: Forward to connected clients
    Debug::log(TRACE, "WolfControlServer: Mouse input: {},{} button={} pressed={}", x, y, button, pressed);
}

void ControlServer::handleKeyboardInput(int keycode, bool pressed) {
    // TODO: Forward to connected clients  
    Debug::log(TRACE, "WolfControlServer: Keyboard input: key={} pressed={}", keycode, pressed);
}

void ControlServer::handleControllerInput(int controller_id, int button, float value) {
    // TODO: Forward to connected clients
    Debug::log(TRACE, "WolfControlServer: Controller input: id={} button={} value={}", controller_id, button, value);
}

void ControlServer::serverThreadMain() {
    Debug::log(LOG, "WolfControlServer: Server thread started");
    
    while (running_) {
        ENetEvent event;
        while (enet_host_service(server_, &event, 10) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                handleClientConnect(event.peer);
                break;
                
            case ENET_EVENT_TYPE_RECEIVE:
                handleClientMessage(event.peer, event.packet->data, event.packet->dataLength);
                enet_packet_destroy(event.packet);
                break;
                
            case ENET_EVENT_TYPE_DISCONNECT:
                handleClientDisconnect(event.peer);
                break;
                
            default:
                break;
            }
        }
    }
    
    Debug::log(LOG, "WolfControlServer: Server thread stopped");
}

void ControlServer::handleClientConnect(ENetPeer* peer) {
    connected_clients_.push_back(peer);
    Debug::log(LOG, "WolfControlServer: Client connected from {}:{}", 
              peer->address.host, peer->address.port);
}

void ControlServer::handleClientDisconnect(ENetPeer* peer) {
    connected_clients_.erase(
        std::remove(connected_clients_.begin(), connected_clients_.end(), peer),
        connected_clients_.end());
    Debug::log(LOG, "WolfControlServer: Client disconnected");
}

void ControlServer::handleClientMessage(ENetPeer* peer, const void* data, size_t size) {
    // TODO: Parse moonlight control protocol messages
    Debug::log(TRACE, "WolfControlServer: Received {} bytes from client", size);
}

// RestServer implementation
RestServer::RestServer(std::shared_ptr<MoonlightState> state)
    : state_(state), running_(false) {
}

RestServer::~RestServer() {
    shutdown();
}

bool RestServer::initialize() {
    Debug::log(LOG, "WolfRestServer: Initializing HTTP server on port {}", state_->getConfig().http_port);
    
    try {
        acceptor_ = std::make_unique<boost::asio::ip::tcp::acceptor>(
            io_context_, 
            boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), state_->getConfig().http_port)
        );
        
        running_ = true;
        server_thread_ = std::thread(&RestServer::serverThreadMain, this);
        
        Debug::log(LOG, "WolfRestServer: Initialized successfully");
        return true;
        
    } catch (const std::exception& e) {
        Debug::log(ERR, "WolfRestServer: Failed to initialize: {}", e.what());
        return false;
    }
}

void RestServer::shutdown() {
    if (running_) {
        running_ = false;
        io_context_.stop();
        if (server_thread_.joinable()) {
            server_thread_.join();
        }
    }
}

void RestServer::handlePairingRequest(const std::string& client_id, const std::string& client_ip) {
    std::lock_guard<std::mutex> lock(pairing_mutex_);
    
    std::string secret = generateSecret();
    
    PairingRequest request;
    request.client_id = client_id;
    request.secret = secret;
    request.client_ip = client_ip;
    request.pin_promise = std::make_shared<std::promise<std::string>>();
    request.timestamp = std::chrono::system_clock::now();
    
    pairing_requests_[secret] = std::move(request);
    
    Debug::log(LOG, "WolfRestServer: Pairing request from {} ({}). Visit http://localhost:{}/pin/#{}", 
              client_id, client_ip, state_->getConfig().http_port, secret);
}

void RestServer::submitPin(const std::string& secret, const std::string& pin) {
    std::lock_guard<std::mutex> lock(pairing_mutex_);
    
    auto it = pairing_requests_.find(secret);
    if (it != pairing_requests_.end()) {
        it->second.pin_promise->set_value(pin);
        Debug::log(LOG, "WolfRestServer: PIN {} submitted for secret {}", pin, secret);
    }
}

void RestServer::serverThreadMain() {
    Debug::log(LOG, "WolfRestServer: Server thread started");
    
    // Simple accept loop
    while (running_) {
        try {
            boost::asio::ip::tcp::socket socket(io_context_);
            acceptor_->accept(socket);
            
            // Handle request in separate thread to avoid blocking
            std::thread([this, socket = std::move(socket)]() mutable {
                handleHttpRequest(std::move(socket));
            }).detach();
            
        } catch (const std::exception& e) {
            if (running_) {
                Debug::log(WARN, "WolfRestServer: Accept error: {}", e.what());
            }
        }
    }
    
    Debug::log(LOG, "WolfRestServer: Server thread stopped");
}

void RestServer::handleHttpRequest(boost::asio::ip::tcp::socket socket) {
    // Basic HTTP request handling - this is very simplified
    try {
        boost::asio::streambuf request;
        boost::asio::read_until(socket, request, "\r\n\r\n");
        
        std::istream request_stream(&request);
        std::string request_line;
        std::getline(request_stream, request_line);
        
        // Parse request line
        std::istringstream iss(request_line);
        std::string method, path, version;
        iss >> method >> path >> version;
        
        Debug::log(TRACE, "WolfRestServer: {} {}", method, path);
        
        std::string response;
        
        if (path == "/serverinfo") {
            response = handleServerInfo();
        } else if (path.starts_with("/pair")) {
            std::map<std::string, std::string> params; // TODO: Parse query parameters
            response = handlePair(params);
        } else if (path.starts_with("/unpair")) {
            std::map<std::string, std::string> params; // TODO: Parse query parameters
            response = handleUnpair(params);
        } else if (path.starts_with("/pin/")) {
            if (method == "GET") {
                response = handlePinPage();
            } else if (method == "POST") {
                // TODO: Read POST body
                response = handlePinSubmit("");
            }
        } else {
            response = "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\n\r\nNot Found";
        }
        
        boost::asio::write(socket, boost::asio::buffer(response));
        
    } catch (const std::exception& e) {
        Debug::log(WARN, "WolfRestServer: Request handling error: {}", e.what());
    }
}

std::string RestServer::generateSecret() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    
    std::stringstream ss;
    for (int i = 0; i < 16; ++i) {
        ss << std::hex << dis(gen);
    }
    return ss.str();
}

std::string RestServer::handleServerInfo() {
    const auto& config = state_->getConfig();
    
    std::stringstream xml;
    xml << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
    xml << "<root status_code=\"200\">";
    xml << "<hostname>" << config.hostname << "</hostname>";
    xml << "<appversion>7.1.431.1</appversion>"; // Mimic GeForce Experience version
    xml << "<GfeVersion>3.27.0.120</GfeVersion>";
    xml << "<uniqueid>" << config.uuid << "</uniqueid>";
    xml << "<MaxLumaPixelsHEVC>1869449984</MaxLumaPixelsHEVC>";
    xml << "<ServerCodecModeSupport>259</ServerCodecModeSupport>";
    xml << "<state>SUNSHINE_STATE_READY</state>";
    xml << "</root>";
    
    std::string body = xml.str();
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: application/xml\r\n";
    response << "Content-Length: " << body.length() << "\r\n";
    response << "\r\n";
    response << body;
    
    return response.str();
}

std::string RestServer::handlePair(const std::map<std::string, std::string>& params) {
    Debug::log(LOG, "[PAIR DEBUG] WolfRestServer: Received pairing request with {} parameters", params.size());

    // Log all parameters for debugging
    for (const auto& param : params) {
        Debug::log(LOG, "[PAIR DEBUG] Parameter: {} = {}", param.first,
                  param.second.length() > 50 ? param.second.substr(0, 50) + "..." : param.second);
    }

    // Extract common parameters
    auto uniqueid = params.find("uniqueid");
    auto salt = params.find("salt");
    auto clientcert = params.find("clientcert");
    auto clientchallenge = params.find("clientchallenge");
    auto serverchallengeresp = params.find("serverchallengeresp");
    auto clientpairingsecret = params.find("clientpairingsecret");

    Debug::log(LOG, "[PAIR DEBUG] Pairing phase detection - uniqueid: {}, salt: {}, clientcert: {}, clientchallenge: {}, serverchallengeresp: {}, clientpairingsecret: {}",
              uniqueid != params.end() ? "YES" : "NO",
              salt != params.end() ? "YES" : "NO",
              clientcert != params.end() ? "YES" : "NO",
              clientchallenge != params.end() ? "YES" : "NO",
              serverchallengeresp != params.end() ? "YES" : "NO",
              clientpairingsecret != params.end() ? "YES" : "NO");

    // For now, return a more detailed failure response that explains the issue
    std::string body;
    if (uniqueid == params.end()) {
        Debug::log(WARN, "[PAIR DEBUG] Missing uniqueid parameter - rejecting pairing request");
        body = "<?xml version=\"1.0\" encoding=\"utf-8\"?><root status_code=\"400\"><paired>0</paired><message>Missing uniqueid parameter</message></root>";
    } else if (salt != params.end() && clientcert != params.end()) {
        Debug::log(INFO, "[PAIR DEBUG] Phase 1 detected - need to implement Wolf pairing integration");
        body = "<?xml version=\"1.0\" encoding=\"utf-8\"?><root status_code=\"400\"><paired>0</paired><message>Wolf pairing integration needed - Phase 1</message></root>";
    } else if (clientchallenge != params.end()) {
        Debug::log(INFO, "[PAIR DEBUG] Phase 2 detected - need to implement Wolf pairing integration");
        body = "<?xml version=\"1.0\" encoding=\"utf-8\"?><root status_code=\"400\"><paired>0</paired><message>Wolf pairing integration needed - Phase 2</message></root>";
    } else if (serverchallengeresp != params.end()) {
        Debug::log(INFO, "[PAIR DEBUG] Phase 3 detected - need to implement Wolf pairing integration");
        body = "<?xml version=\"1.0\" encoding=\"utf-8\"?><root status_code=\"400\"><paired>0</paired><message>Wolf pairing integration needed - Phase 3</message></root>";
    } else if (clientpairingsecret != params.end()) {
        Debug::log(INFO, "[PAIR DEBUG] Phase 4 detected - need to implement Wolf pairing integration");
        body = "<?xml version=\"1.0\" encoding=\"utf-8\"?><root status_code=\"400\"><paired>0</paired><message>Wolf pairing integration needed - Phase 4</message></root>";
    } else {
        Debug::log(WARN, "[PAIR DEBUG] Unknown pairing phase - parameters don't match expected patterns");
        body = "<?xml version=\"1.0\" encoding=\"utf-8\"?><root status_code=\"400\"><paired>0</paired><message>Unknown pairing phase</message></root>";
    }

    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: application/xml\r\n";
    response << "Content-Length: " << body.length() << "\r\n";
    response << "\r\n";
    response << body;

    return response.str();
}

std::string RestServer::handleUnpair(const std::map<std::string, std::string>& params) {
    std::string body = "<?xml version=\"1.0\" encoding=\"utf-8\"?><root status_code=\"200\"></root>";
    
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: application/xml\r\n";
    response << "Content-Length: " << body.length() << "\r\n";
    response << "\r\n";
    response << body;
    
    return response.str();
}

std::string RestServer::handlePinPage() {
    std::string html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Moonlight Pairing</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 50px; }
        .container { max-width: 400px; margin: 0 auto; text-align: center; }
        input[type="text"] { padding: 10px; font-size: 18px; width: 200px; }
        button { padding: 10px 20px; font-size: 16px; margin-left: 10px; }
    </style>
</head>
<body>
    <div class="container">
        <h1>Moonlight Pairing</h1>
        <p>Enter the 4-digit PIN shown on your Moonlight client:</p>
        <form id="pinForm">
            <input type="text" id="pin" maxlength="4" placeholder="PIN" autofocus>
            <button type="submit">Submit</button>
        </form>
    </div>
    <script>
        document.getElementById('pinForm').onsubmit = function(e) {
            e.preventDefault();
            var pin = document.getElementById('pin').value;
            if (pin.length === 4) {
                fetch('/pin/', {
                    method: 'POST',
                    headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify({pin: pin, secret: window.location.hash.substring(1)})
                }).then(() => {
                    alert('PIN submitted successfully!');
                });
            }
        };
    </script>
</body>
</html>
)";
    
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << html.length() << "\r\n";
    response << "\r\n";
    response << html;
    
    return response.str();
}

std::string RestServer::handlePinSubmit(const std::string& body) {
    // TODO: Parse JSON body and extract PIN
    std::string response_body = "OK";
    
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/plain\r\n";
    response << "Content-Length: " << response_body.length() << "\r\n";
    response << "\r\n";
    response << response_body;
    
    return response.str();
}

// WolfMoonlightServer implementation
WolfMoonlightServer::WolfMoonlightServer() : http_server_(nullptr), https_server_(nullptr) {
}

WolfMoonlightServer::~WolfMoonlightServer() {
    shutdown();
}

bool WolfMoonlightServer::initialize(const MoonlightConfig& config) {
    logs::log(logs::warning, "WolfMoonlightServer: Initializing moonlight server");

    if (!initializeGStreamer()) {
        Debug::log(ERR, "WolfMoonlightServer: Failed to initialize GStreamer");
        return false;
    }
    
    if (!initializeENet()) {
        Debug::log(ERR, "WolfMoonlightServer: Failed to initialize ENet");
        return false;
    }
    
    // Create components
    state_ = std::make_shared<MoonlightState>(config);
    if (!state_->initialize()) {
        Debug::log(ERR, "WolfMoonlightServer: Failed to initialize state");
        return false;
    }
    
    streaming_engine_ = std::make_unique<StreamingEngine>(state_);
    if (!streaming_engine_->initialize()) {
        Debug::log(ERR, "WolfMoonlightServer: Failed to initialize streaming engine");
        return false;
    }
    
    control_server_ = std::make_unique<ControlServer>(state_);
    if (!control_server_->initialize()) {
        Debug::log(ERR, "WolfMoonlightServer: Failed to initialize control server");
        return false;
    }
    
    // Replace stub REST server with real Wolf HTTP/HTTPS servers
    Debug::log(LOG, "WolfMoonlightServer: Initializing real Wolf HTTP/HTTPS servers");

    // Initialize Wolf AppState for real endpoints
    initializeWolfAppState();

    // CRITICAL: Generate certificates FIRST, before starting any servers
    generateAndLoadCertificates();

    // Start HTTP server (port 47989) using real Wolf implementation - NOW with certificates
    http_server_ = new HttpServer();
    initializeHttpServer();

    // Start HTTPS server (port 47984) using real Wolf implementation - with certificates
    https_server_ = nullptr; // Will be created in initializeHttpsServer with proper certs
    initializeHttpsServer();
    
    initialized_ = true;
    Debug::log(LOG, "WolfMoonlightServer: Moonlight server ready on ports HTTP:{}, Control:{}, Video:{}", 
              config.http_port, config.control_port, config.video_port);
    
    return true;
}

void WolfMoonlightServer::shutdown() {
    // Thread-safe shutdown with double-call protection
    std::lock_guard<std::mutex> lock(shutdown_mutex_);

    if (!initialized_) {
        Debug::log(LOG, "WolfMoonlightServer: Already shut down, skipping");
        return;
    }

    Debug::log(LOG, "WolfMoonlightServer: Shutting down moonlight server");

    // Shutdown HTTP server with safe deletion
    if (http_server_) {
        try {
            auto server = std::unique_ptr<HttpServer>(static_cast<HttpServer*>(http_server_));
            http_server_ = nullptr; // Clear pointer immediately
            server->stop();
            if (http_thread_.joinable()) {
                http_thread_.join();
            }
            // server auto-deletes via unique_ptr
        } catch (const std::exception& e) {
            Debug::log(ERR, "WolfMoonlightServer: Error shutting down HTTP server: {}", e.what());
        }
    }

    // Shutdown HTTPS server with safe deletion
    if (https_server_) {
        try {
            auto server = std::unique_ptr<HttpsServer>(static_cast<HttpsServer*>(https_server_));
            https_server_ = nullptr; // Clear pointer immediately
            server->stop();
            if (https_thread_.joinable()) {
                https_thread_.join();
            }
            // server auto-deletes via unique_ptr
        } catch (const std::exception& e) {
            Debug::log(ERR, "WolfMoonlightServer: Error shutting down HTTPS server: {}", e.what());
        }
    }

    // Safe reset of smart pointers
    try {
        wolf_app_state_.reset();
        control_server_.reset();
        streaming_engine_.reset();
        state_.reset();
    } catch (const std::exception& e) {
        Debug::log(ERR, "WolfMoonlightServer: Error during component cleanup: {}", e.what());
    }

    initialized_ = false;
    Debug::log(LOG, "WolfMoonlightServer: Shutdown completed safely");
}

void WolfMoonlightServer::onFrameReady(const void* frame_data, size_t size, int width, int height, uint32_t format) {
    if (streaming_engine_) {
        streaming_engine_->pushFrame(frame_data, size, width, height, format);
    }
}

void WolfMoonlightServer::onFrameReadyDMABuf(int dmabuf_fd, uint32_t stride, uint64_t modifier, int width, int height, uint32_t format, wlr_buffer* buffer_ref) {
    if (streaming_engine_) {
        streaming_engine_->pushFrameDMABuf(dmabuf_fd, stride, modifier, width, height, format, buffer_ref);
    }
}

void WolfMoonlightServer::updateConfig(const MoonlightConfig& config) {
    if (state_) {
        state_->updateConfig(config);
    }
}

const MoonlightConfig& WolfMoonlightServer::getConfig() const {
    static MoonlightConfig default_config;
    return state_ ? state_->getConfig() : default_config;
}

bool WolfMoonlightServer::isStreaming() const {
    // Check if any session is active
    if (!state_) return false;
    
    auto clients = state_->getPairedClients();
    return !clients.empty(); // Simplified check
}

int WolfMoonlightServer::getConnectedClientCount() const {
    if (!state_) return 0;
    return state_->getPairedClients().size();
}

std::vector<HyprlandPairedClient> WolfMoonlightServer::getPairedClients() const {
    if (!state_) return {};
    return state_->getPairedClients();
}

void WolfMoonlightServer::unpairClient(const std::string& client_id) {
    if (state_) {
        state_->removePairedClient(client_id);
    }
}

bool WolfMoonlightServer::initializeGStreamer() {
    if (!gst_is_initialized()) {
        gst_init(nullptr, nullptr);
        Debug::log(LOG, "WolfMoonlightServer: GStreamer initialized");
    }
    return true;
}

bool WolfMoonlightServer::initializeENet() {
    // ENet initialization is handled by ControlServer
    return true;
}

void WolfMoonlightServer::initializeWolfAppState() {
    Debug::log(LOG, "WolfMoonlightServer: Initializing Wolf AppState for real endpoints");

    // Create minimal Wolf AppState for endpoints to work
    // Note: This is a simplified version - real Wolf has more complex state management
    auto app_state = std::make_shared<state::AppState>();

    // Initialize basic configuration
    state::Config config;
    config.hostname = state_->getConfig().hostname;
    config.uuid = state_->getConfig().uuid;
    config.support_hevc = true;
    config.support_av1 = false;

    // CRITICAL: Initialize paired_clients to prevent NULL pointer crashes
    config.config_source = "/tmp/hyprmoon-config.toml";
    auto empty_paired_clients = state::PairedClientList{};
    config.paired_clients = std::make_shared<immer::atom<state::PairedClientList>>(empty_paired_clients);
    logs::log(logs::warning, "WolfMoonlightServer: Initialized Config with empty paired_clients (prevents NULL crashes)");

    app_state->config = immer::box<state::Config>(config);

    // Initialize host information with proper certificate paths
    state::Host host_info;
    host_info.internal_ip = "127.0.0.1"; // Will be updated when server starts
    host_info.mac_address = "00:00:00:00:00:00"; // Default MAC

    // Initialize display modes
    moonlight::DisplayMode default_mode{2360, 1640, 120, true, false};
    host_info.display_modes = immer::array<moonlight::DisplayMode>{default_mode};

    // CRITICAL: Initialize certificate paths for pairing
    // These will be loaded when HTTPS server initializes certificates
    cert_file_path_ = "/tmp/moonlight-cert.pem";
    key_file_path_ = "/tmp/moonlight-key.pem";

    Debug::log(LOG, "WolfMoonlightServer: Will load certificates from {} and {}", cert_file_path_, key_file_path_);

    app_state->host = immer::box<state::Host>(host_info);

    // Initialize pairing cache
    app_state->pairing_cache = std::make_shared<immer::atom<immer::map<std::string, state::PairCache>>>();

    // Initialize event bus for pairing signals
    app_state->event_bus = std::make_shared<events::EventBusType>();

    // Initialize pairing atom for PIN handling
    app_state->pairing_atom = std::make_shared<immer::atom<immer::map<std::string, immer::box<events::PairSignal>>>>();

    // Initialize running sessions
    app_state->running_sessions = std::make_shared<immer::atom<immer::vector<events::StreamSession>>>();

    wolf_app_state_ = app_state;
    Debug::log(LOG, "WolfMoonlightServer: Wolf AppState initialized successfully");
}

void WolfMoonlightServer::initializeHttpServer() {
    Debug::log(LOG, "WolfMoonlightServer: Starting real Wolf HTTP server on port {}", state_->getConfig().http_port);

    auto server = static_cast<HttpServer*>(http_server_);

    // Use the real Wolf HTTP server implementation from servers.cpp
    http_thread_ = std::thread([this, server, port = state_->getConfig().http_port]() {
        // Get fresh AppState inside thread (though HTTP doesn't need certificates for pairing)
        auto fresh_app_state = std::static_pointer_cast<state::AppState>(wolf_app_state_);
        HTTPServers::startServer(server, immer::box<state::AppState>(*fresh_app_state), port);
    });

    Debug::log(LOG, "WolfMoonlightServer: HTTP server thread started");
}

void WolfMoonlightServer::generateAndLoadCertificates() {
    logs::log(logs::warning, "WolfMoonlightServer: Generating and loading certificates for both HTTP and HTTPS servers");

    // Create self-signed certificate files for moonlight
    std::string cert_file = "/tmp/moonlight-cert.pem";
    std::string key_file = "/tmp/moonlight-key.pem";

    // Generate self-signed certificate command
    std::string gen_cert_cmd = "openssl req -x509 -newkey rsa:2048 -keyout " + key_file +
                              " -out " + cert_file + " -days 365 -nodes -subj '/CN=localhost'";

    logs::log(logs::warning, "WolfMoonlightServer: Executing certificate generation command: {}", gen_cert_cmd);
    int cert_result = system(gen_cert_cmd.c_str());
    logs::log(logs::warning, "WolfMoonlightServer: Certificate generation result: {}", cert_result);

    if (cert_result == 0) {
        logs::log(logs::warning, "WolfMoonlightServer: Generated self-signed certificates");

        // CRITICAL: Load certificates into Wolf AppState for ALL servers
        loadCertificatesIntoAppState(cert_file, key_file);
    } else {
        logs::log(logs::error, "WolfMoonlightServer: CRITICAL - Certificate generation failed with exit code: {}", cert_result);
        logs::log(logs::error, "WolfMoonlightServer: Command was: {}", gen_cert_cmd);
        logs::log(logs::error, "WolfMoonlightServer: This means servers will not have valid certificates for pairing!");
    }
}

void WolfMoonlightServer::initializeHttpsServer() {
    logs::log(logs::warning, "WolfMoonlightServer: Initializing HTTPS server (certificates already loaded)");

    // Thread-safe server initialization
    std::lock_guard<std::mutex> lock(shutdown_mutex_);

    // Use the certificates that were already generated
    std::string cert_file = "/tmp/moonlight-cert.pem";
    std::string key_file = "/tmp/moonlight-key.pem";

        try {
            // Create HTTPS server with certificates
            auto new_server = std::make_unique<HttpsServer>(cert_file, key_file);

            // Clean up old server safely
            if (https_server_) {
                auto old_server = std::unique_ptr<HttpsServer>(static_cast<HttpsServer*>(https_server_));
                // old_server auto-deletes
            }

            https_server_ = new_server.release();

            // Start HTTPS server with updated AppState (AFTER certificate loading)
            logs::log(logs::warning, "WolfMoonlightServer: Getting FRESH AppState for HTTPS server AFTER certificate loading");
            auto server = static_cast<HttpsServer*>(https_server_);

            https_thread_ = std::thread([this, server, port = state_->getConfig().https_port]() {
                // Get AppState INSIDE the thread to ensure we get the updated version
                auto fresh_app_state = std::static_pointer_cast<state::AppState>(wolf_app_state_);
                logs::log(logs::warning, "HTTPServers: Using fresh AppState with certificates loaded");
                HTTPServers::startServer(server, immer::box<state::AppState>(*fresh_app_state), port);
            });

            Debug::log(LOG, "WolfMoonlightServer: HTTPS server initialized and started with certificates");

        } catch (const std::exception& e) {
            Debug::log(ERR, "WolfMoonlightServer: Failed to create HTTPS server: {}", e.what());
            https_server_ = nullptr;
        }
}

void WolfMoonlightServer::loadCertificatesIntoAppState(const std::string& cert_file, const std::string& key_file) {
    logs::log(logs::warning, "WolfMoonlightServer: Loading certificates into Wolf AppState from {} and {}", cert_file, key_file);

    auto app_state = std::static_pointer_cast<state::AppState>(wolf_app_state_);

    try {
        // Load certificate using Wolf's x509 functions
        logs::log(logs::warning, "WolfMoonlightServer: About to load certificate from {}", cert_file);
        auto server_cert = x509::cert_from_file(cert_file);
        if (!server_cert) {
            logs::log(logs::error, "WolfMoonlightServer: Failed to load certificate from {}", cert_file);
            return;
        }
        logs::log(logs::warning, "WolfMoonlightServer: Certificate loaded successfully from {}", cert_file);

        // Load private key using Wolf's x509 functions
        logs::log(logs::warning, "WolfMoonlightServer: About to load private key from {}", key_file);
        auto server_pkey = x509::pkey_from_file(key_file);
        if (!server_pkey) {
            logs::log(logs::error, "WolfMoonlightServer: Failed to load private key from {}", key_file);
            return;
        }
        logs::log(logs::warning, "WolfMoonlightServer: Private key loaded successfully from {}", key_file);

        logs::log(logs::warning, "WolfMoonlightServer: Successfully loaded certificate and private key from files");

        // Update the host info in AppState with the loaded certificates
        logs::log(logs::warning, "WolfMoonlightServer: About to update AppState with certificates");
        auto current_host = app_state->host.get();
        logs::log(logs::warning, "WolfMoonlightServer: Retrieved current host from AppState");
        state::Host updated_host = current_host;

        // CRITICAL: Set the server certificate and private key that pairing endpoints expect
        logs::log(logs::warning, "WolfMoonlightServer: Setting server_cert and server_pkey in updated_host");
        updated_host.server_cert = server_cert;
        updated_host.server_pkey = server_pkey;

        // Update AppState with new host info containing certificates
        logs::log(logs::warning, "WolfMoonlightServer: Updating AppState host with new certificates");
        app_state->host = immer::box<state::Host>(updated_host);
        logs::log(logs::warning, "WolfMoonlightServer: AppState host updated successfully - certificates should now be available for pairing");

        logs::log(logs::warning, "WolfMoonlightServer: CERTIFICATES LOADED INTO WOLF APPSTATE - pairing should now work!");

        // Verify the certificates are properly loaded by testing the pairing functions
        try {
            auto test_pem = x509::get_cert_pem(server_cert);
            Debug::log(LOG, "WolfMoonlightServer: Certificate PEM verification - length: {}, first 50 chars: {}",
                      test_pem.length(),
                      test_pem.length() > 0 ? test_pem.substr(0, std::min(50ul, test_pem.length())) : "EMPTY");
        } catch (const std::exception& e) {
            Debug::log(ERR, "WolfMoonlightServer: Certificate PEM test failed: {}", e.what());
        }

    } catch (const std::exception& e) {
        Debug::log(ERR, "WolfMoonlightServer: Exception loading certificates into AppState: {}", e.what());
    }
}

} // namespace core
} // namespace wolf