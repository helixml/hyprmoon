#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <thread>
#include <atomic>
#include <chrono>

// Minimal Wolf-compatible moonlight server implementation
// This provides the essential functionality without all of Wolf's dependencies

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <boost/asio.hpp>
#include <enet/enet.h>

// Forward declarations for Wolf AppState
#include "../state/data-structures.hpp"

namespace wolf {
namespace core {

// Forward declarations
class MoonlightState;
class StreamingEngine;
class ControlServer;
class RestServer;

// Configuration structure
struct MoonlightConfig {
    std::string uuid = "hyprland-moonlight-server";
    std::string hostname = "Hyprland";
    int http_port = 47989;
    int https_port = 47984;
    int rtsp_port = 48010;
    int control_port = 47999;
    int video_port = 48000;
    int audio_port = 48002;
    bool support_hevc = true;
    bool support_av1 = false;
    
    struct VideoConfig {
        int width = 1920;
        int height = 1080;
        int fps = 60;
        int bitrate = 20000; // kbps
        std::string encoder = "auto"; // nvenc, vaapi, qsv, x264
    } video;
    
    struct AudioConfig {
        int channels = 2;
        int sample_rate = 48000;
        int bitrate = 128; // kbps
    } audio;
};

// Client information - renamed to avoid namespace conflict with Wolf's PairedClient
struct HyprlandPairedClient {
    std::string client_cert;
    std::string client_name;
    std::string client_id;
    std::chrono::system_clock::time_point paired_time;
};

// Session state
struct StreamSession {
    std::string session_id;
    HyprlandPairedClient client;
    std::string client_ip;
    uint16_t video_port;
    uint16_t audio_port;
    std::atomic<bool> active{false};
    std::chrono::system_clock::time_point start_time;
};

// Frame callback for Hyprland integration
using FrameCallback = std::function<void(const void* frame_data, size_t size, int width, int height, uint32_t format)>;

class MoonlightState {
public:
    MoonlightState(const MoonlightConfig& config);
    ~MoonlightState();
    
    bool initialize();
    void shutdown();
    
    // Configuration
    const MoonlightConfig& getConfig() const { return config_; }
    void updateConfig(const MoonlightConfig& config) { config_ = config; }
    
    // Client management
    void addPairedClient(const HyprlandPairedClient& client);
    void removePairedClient(const std::string& client_id);
    std::vector<HyprlandPairedClient> getPairedClients() const;

    // Session management
    std::shared_ptr<StreamSession> createSession(const HyprlandPairedClient& client, const std::string& client_ip);
    void destroySession(const std::string& session_id);
    std::shared_ptr<StreamSession> getSession(const std::string& session_id);
    
private:
    MoonlightConfig config_;
    std::vector<HyprlandPairedClient> paired_clients_;
    std::map<std::string, std::shared_ptr<StreamSession>> active_sessions_;
    mutable std::mutex mutex_;
};

class StreamingEngine {
public:
    StreamingEngine(std::shared_ptr<MoonlightState> state);
    ~StreamingEngine();
    
    bool initialize();
    void shutdown();
    
    // Frame input from Hyprland
    void pushFrame(const void* frame_data, size_t size, int width, int height, uint32_t format);
    void pushFrameDMABuf(int dmabuf_fd, uint32_t stride, uint64_t modifier, int width, int height, uint32_t format, wlr_buffer* buffer_ref);
    
    // Session control
    bool startStreaming(const std::string& session_id);
    void stopStreaming(const std::string& session_id);
    bool isStreaming(const std::string& session_id) const;
    
private:
    std::shared_ptr<MoonlightState> state_;
    
    // GStreamer pipeline components
    GstElement* pipeline_;
    GstElement* app_src_;
    GstElement* encoder_;
    GstElement* payloader_;
    GstElement* sink_;
    
    // Threading
    std::thread gst_thread_;
    std::atomic<bool> running_;
    
    // Internal methods
    bool setupGStreamerPipeline();
    void cleanupGStreamerPipeline();
    void gstreamerThreadMain();
    std::string buildPipelineDescription() const;
};

class ControlServer {
public:
    ControlServer(std::shared_ptr<MoonlightState> state);
    ~ControlServer();
    
    bool initialize();
    void shutdown();
    
    // Input handling
    void handleMouseInput(int x, int y, int button, bool pressed);
    void handleKeyboardInput(int keycode, bool pressed);
    void handleControllerInput(int controller_id, int button, float value);
    
private:
    std::shared_ptr<MoonlightState> state_;
    
    // ENet server
    ENetHost* server_;
    std::vector<ENetPeer*> connected_clients_;
    
    // Threading
    std::thread server_thread_;
    std::atomic<bool> running_;
    
    // Internal methods
    void serverThreadMain();
    void handleClientConnect(ENetPeer* peer);
    void handleClientDisconnect(ENetPeer* peer);
    void handleClientMessage(ENetPeer* peer, const void* data, size_t size);
};

class RestServer {
public:
    RestServer(std::shared_ptr<MoonlightState> state);
    ~RestServer();
    
    bool initialize();
    void shutdown();
    
    // Pairing
    struct PairingRequest {
        std::string client_id;
        std::string secret;
        std::string client_ip;
        std::shared_ptr<std::promise<std::string>> pin_promise;
        std::chrono::system_clock::time_point timestamp;
    };
    
    void handlePairingRequest(const std::string& client_id, const std::string& client_ip);
    void submitPin(const std::string& secret, const std::string& pin);
    
private:
    std::shared_ptr<MoonlightState> state_;
    
    // HTTP server (using boost::beast)
    boost::asio::io_context io_context_;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
    
    // Threading
    std::thread server_thread_;
    std::atomic<bool> running_;
    
    // Active pairing requests
    std::map<std::string, PairingRequest> pairing_requests_;
    mutable std::mutex pairing_mutex_;
    
    // Internal methods
    void serverThreadMain();
    void handleHttpRequest(boost::asio::ip::tcp::socket socket);
    std::string generateSecret();
    std::string handleServerInfo();
    std::string handlePair(const std::map<std::string, std::string>& params);
    std::string handleUnpair(const std::map<std::string, std::string>& params);
    std::string handlePinPage();
    std::string handlePinSubmit(const std::string& body);
};

// Main moonlight server class that coordinates all components
class WolfMoonlightServer {
public:
    WolfMoonlightServer();
    ~WolfMoonlightServer();
    
    bool initialize(const MoonlightConfig& config = MoonlightConfig{});
    void shutdown();
    
    // Frame input from Hyprland renderer
    void onFrameReady(const void* frame_data, size_t size, int width, int height, uint32_t format);
    void onFrameReadyDMABuf(int dmabuf_fd, uint32_t stride, uint64_t modifier, int width, int height, uint32_t format, wlr_buffer* buffer_ref);
    
    // Configuration
    void updateConfig(const MoonlightConfig& config);
    const MoonlightConfig& getConfig() const;
    
    // Status
    bool isInitialized() const { return initialized_; }
    bool isStreaming() const;
    int getConnectedClientCount() const;
    
    // Client management
    std::vector<HyprlandPairedClient> getPairedClients() const;
    void unpairClient(const std::string& client_id);
    
private:
    std::atomic<bool> initialized_{false};
    
    // Core components
    std::shared_ptr<MoonlightState> state_;
    std::unique_ptr<StreamingEngine> streaming_engine_;
    std::unique_ptr<ControlServer> control_server_;

    // Wolf AppState - keep as void* for compatibility, cast when needed
    std::shared_ptr<void> wolf_app_state_;
    void* http_server_;
    void* https_server_;
    std::thread http_thread_;
    std::thread https_thread_;

    // Thread safety
    mutable std::mutex shutdown_mutex_;

    // Certificate paths for loading into AppState
    std::string cert_file_path_;
    std::string key_file_path_;

    // Active streaming session tracking for frame routing
    std::string current_session_id_;

    // Event handler registrations (must be stored to keep them alive)
    std::vector<void*> event_handlers_;

    // Initialization
    bool initializeGStreamer();
    bool initializeENet();
    void initializeWolfAppState();
    void generateAndLoadCertificates();
    void initializeHttpServer();
    void initializeHttpsServer();
    void loadCertificatesIntoAppState(const std::string& cert_file, const std::string& key_file);
    void registerStreamingEventHandlers(std::shared_ptr<state::AppState> app_state);
    void startRTPPingServers(std::shared_ptr<state::AppState> app_state);
    void startSimpleRTSPServer(std::shared_ptr<state::AppState> app_state);
    void startControlServer(std::shared_ptr<state::AppState> app_state);
    void startMDNSService(std::shared_ptr<state::AppState> app_state);
};

} // namespace core
} // namespace wolf