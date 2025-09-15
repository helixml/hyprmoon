#pragma once
#include "../helpers/memory/Memory.hpp"
// #include "rest/rest.hpp"  // TODO: Fix SimpleWeb-Server dependency - use forward declarations for now
// #include "state/data-structures.hpp"  // TODO: Fix circular includes
// #include "streaming/StreamingManager.hpp"  // TODO: Add when streaming is implemented
// #include "input/InputManager.hpp"  // TODO: Add when input is implemented
#include <memory>
#include <string>
#include <vector>

// Forward declarations to avoid problematic includes
class HttpServer;
class HttpsServer;
namespace moonlight {
namespace rest {
    struct ClientInfo;  // Forward declaration instead of including rest.hpp
}
namespace streaming {
    class StreamingManager;
}
namespace input {
    class InputManager;
}
}
namespace state {
    class AppState;
}

class CMoonlightManager {
public:
    CMoonlightManager();
    ~CMoonlightManager();

    void init();
    void destroy();

    // REST API management
    bool startRestAPI();
    void stopRestAPI();
    bool isRestAPIRunning() const;

    // Client management
    void addPairedClient(const moonlight::rest::ClientInfo& client);
    void removePairedClient(const std::string& client_id);
    std::vector<moonlight::rest::ClientInfo> getPairedClients() const;

    // Step 6: Streaming management
    bool startStreaming(const std::string& client_ip, unsigned short video_port, unsigned short audio_port);
    bool stopStreaming();
    bool isStreaming() const;

    // Step 7: Input management
    bool startInputManager();
    void stopInputManager();
    bool isInputManagerRunning() const;

    // Input event handlers for Moonlight clients
    void handleMouseMove(float x, float y, bool relative = false);
    void handleMouseButton(int button, bool pressed);
    void handleMouseScroll(float scrollX, float scrollY);
    void handleKeyboardKey(int keycode, bool pressed);
    void handleTouchEvent(int touchId, float x, float y, bool pressed);

    // Frame capture (Step 5) - enhanced with streaming in Step 6
    void onFrameReady(class CMonitor* monitor, struct wlr_buffer* buffer);

private:
    bool m_bEnabled = false;

    // Wolf REST API servers (HTTP and HTTPS)
    std::unique_ptr<HttpServer> m_httpServer;
    std::unique_ptr<HttpsServer> m_httpsServer;

    // Wolf application state for REST API (TODO: implement when state system is ready)
    // immer::box<state::AppState> m_appState;

    // Step 6: Streaming infrastructure
    std::unique_ptr<moonlight::streaming::StreamingManager> m_streamingManager;

    // Step 7: Input management infrastructure
    std::unique_ptr<moonlight::input::InputManager> m_inputManager;

    // Helper methods for Wolf REST API setup
    void setupWolfAppState();
    void generateSelfSignedCertificate(const std::string& cert_file, const std::string& key_file);
};

// Global instance
extern UP<CMoonlightManager> g_pMoonlightManager;