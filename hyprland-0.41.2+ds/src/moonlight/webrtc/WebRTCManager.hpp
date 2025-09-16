#pragma once

#define GST_USE_UNSTABLE_API
#include <gst/gst.h>
#include <gst/webrtc/webrtc.h>
#include <string>
#include <memory>
#include <functional>
#include <atomic>
#include <mutex>
#include <map>

// Forward declarations
class CMoonlightManager;

/**
 * WebRTC Manager - Provides WebRTC streaming alongside Wolf/Moonlight
 * 
 * This class creates a parallel streaming path using GStreamer's WebRTC
 * implementation, allowing browser-based clients to connect while maintaining
 * the existing Wolf/Moonlight protocol for gaming clients.
 */
class CWebRTCManager {
public:
    CWebRTCManager();
    ~CWebRTCManager();
    
    // Lifecycle
    bool initialize();
    void shutdown();
    
    // Pipeline integration
    bool connectToFrameSource(GstElement* tee_element);
    void disconnectFromFrameSource();
    
    // WebRTC session management
    std::string createOffer(const std::string& clientId);
    bool setRemoteAnswer(const std::string& clientId, const std::string& sdp);
    bool addICECandidate(const std::string& clientId, const std::string& candidate);
    void closeSession(const std::string& clientId);
    
    // Session info
    bool hasActiveSession(const std::string& clientId) const;
    std::vector<std::string> getActiveClients() const;
    size_t getClientCount() const;
    
    // Configuration
    struct Config {
        std::string stun_server = "stun://stun.l.google.com:19302";
        std::vector<std::string> turn_servers;
        
        // Video settings
        int max_video_bitrate = 8000;  // kbps
        int min_video_bitrate = 500;   // kbps
        std::string video_codec = "H264";  // H264, VP8, VP9
        
        // Audio settings
        bool enable_audio_input = true;   // Microphone from browser
        bool enable_audio_output = true;  // Desktop audio to browser
        int audio_sample_rate = 48000;
        int audio_channels = 2;
        std::string audio_codec = "OPUS";  // OPUS, G722
        
        // Input settings
        bool enable_keyboard_input = true;
        bool enable_mouse_input = true;
        bool enable_gamepad_input = true;
        
        // Security
        bool require_user_gesture = true;  // For audio/input permissions
    } m_config;
    
    // Callbacks for signaling (to be handled by REST server)
    std::function<void(const std::string& clientId, const std::string& offer)> onOfferCreated;
    std::function<void(const std::string& clientId, const std::string& answer)> onAnswerCreated;
    std::function<void(const std::string& clientId, const std::string& candidate)> onICECandidate;
    std::function<void(const std::string& clientId, bool connected)> onConnectionStateChanged;
    
    // Audio callbacks
    std::function<void(const void* audio_data, size_t size, int channels, int sample_rate)> onAudioReceived;
    
    // Input callbacks (forwarded to Hyprland input system)
    std::function<void(int keycode, bool pressed, uint32_t modifiers)> onKeyboardInput;
    std::function<void(double x, double y, int button, bool pressed)> onMouseInput;
    std::function<void(double x, double y)> onMouseMove;
    std::function<void(double delta_x, double delta_y)> onMouseScroll;

private:
    // WebRTC session data
    struct WebRTCSession {
        std::string client_id;
        
        // Video pipeline
        GstElement* webrtcbin = nullptr;
        GstElement* video_queue = nullptr;
        GstElement* videoconvert = nullptr;
        
        // Audio pipeline (if enabled)
        GstElement* audio_src = nullptr;     // PulseAudio source
        GstElement* audio_sink = nullptr;    // For received audio
        GstElement* audio_queue_in = nullptr;
        GstElement* audio_queue_out = nullptr;
        
        // Data channels for input
        gint input_channel_id = -1;
        bool input_channel_ready = false;
        
        // Session state
        bool offer_created = false;
        bool connected = false;
        bool permissions_granted = false;  // Audio/input permissions
        std::chrono::steady_clock::time_point created_time;
        
        // Input state tracking
        struct {
            bool mouse_captured = false;
            double last_mouse_x = 0.0;
            double last_mouse_y = 0.0;
            uint32_t active_keys = 0;  // Bitmask of pressed keys
        } input_state;
    };
    
    // State
    std::atomic<bool> m_initialized{false};
    std::atomic<bool> m_connected_to_source{false};
    
    // GStreamer elements
    GstElement* m_pipeline = nullptr;
    GstElement* m_source_tee = nullptr;  // Reference to shared tee
    
    // Session management
    mutable std::mutex m_sessions_mutex;
    std::map<std::string, std::unique_ptr<WebRTCSession>> m_sessions;
    
    // Internal methods
    bool setupBasePipeline();
    void cleanupPipeline();
    
    std::unique_ptr<WebRTCSession> createSession(const std::string& clientId);
    void destroySession(const std::string& clientId);
    WebRTCSession* getSession(const std::string& clientId);
    
    bool linkSessionToPipeline(WebRTCSession* session);
    void unlinkSessionFromPipeline(WebRTCSession* session);
    bool setupAudioPipeline(WebRTCSession* session, GstElement* main_pipeline);
    
    // GStreamer callback handlers
    void onNegotiationNeeded(GstElement* webrtcbin, const std::string& clientId);
    void onOfferCreatedCallback(GstPromise* promise, const std::string& clientId);
    void onAnswerCreatedCallback(GstPromise* promise, const std::string& clientId);
    void onICECandidateCallback(GstElement* webrtcbin, guint mlineindex, 
                               const gchar* candidate, const std::string& clientId);
    void onConnectionStateChangedCallback(GstElement* webrtcbin, 
                                        GParamSpec* pspec, const std::string& clientId);
    
    // Audio callbacks
    void onAudioSample(GstElement* appsink, const std::string& clientId);
    
    // Data channel callbacks (for input)
    void onDataChannelOpen(GstWebRTCDataChannel* channel, const std::string& clientId);
    void onDataChannelMessage(GstWebRTCDataChannel* channel, GBytes* data, const std::string& clientId);
    void onDataChannelClose(GstWebRTCDataChannel* channel, const std::string& clientId);
    
    // Input processing
    void processInputMessage(const std::string& clientId, const std::string& message);
    void processKeyboardInput(const std::string& clientId, int keycode, bool pressed, uint32_t modifiers);
    void processMouseInput(const std::string& clientId, double x, double y, int button, bool pressed);
    void processMouseMove(const std::string& clientId, double x, double y);
    void processMouseScroll(const std::string& clientId, double delta_x, double delta_y);
    void processTouchInput(const std::string& clientId, double x, double y, bool pressed, int touch_id);
    
    // Static callbacks for GStreamer C API
    static void on_negotiation_needed_static(GstElement* element, gpointer user_data);
    static void on_offer_created_static(GstPromise* promise, gpointer user_data);
    static void on_answer_created_static(GstPromise* promise, gpointer user_data);
    static void on_ice_candidate_static(GstElement* webrtc, guint mlineindex, 
                                       gchar* candidate, gpointer user_data);
    static void on_connection_state_changed_static(GstElement* webrtcbin, 
                                                  GParamSpec* pspec, gpointer user_data);
    
    // Audio static callbacks
    static GstFlowReturn on_audio_sample_static(GstElement* appsink, gpointer user_data);
    
    // Data channel static callbacks
    static void on_data_channel_open_static(GstWebRTCDataChannel* channel, gpointer user_data);
    static void on_data_channel_message_static(GstWebRTCDataChannel* channel, GBytes* data, gpointer user_data);
    static void on_data_channel_close_static(GstWebRTCDataChannel* channel, gpointer user_data);
    static void on_data_channel_created_static(GstElement* webrtcbin, GstWebRTCDataChannel* channel, gpointer user_data);
    
    // Utility methods
    std::string generateSessionId();
    void cleanupExpiredSessions();
    GstWebRTCSessionDescription* createSessionDescription(const std::string& sdp, 
                                                         GstWebRTCSDPType type);
    
    // Session data storage for callbacks
    struct CallbackData {
        CWebRTCManager* manager;
        std::string client_id;
    };
    std::map<gpointer, std::unique_ptr<CallbackData>> m_callback_data;
    std::mutex m_callback_mutex;
    
    void storeCallbackData(gpointer key, const std::string& clientId);
    std::string getCallbackClientId(gpointer key);
    void removeCallbackData(gpointer key);
};