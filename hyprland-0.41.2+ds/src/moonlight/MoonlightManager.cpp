#include "MoonlightManager.hpp"
#include "crypto/CryptoStub.hpp"
#include "protocol/RTSPStub.hpp"
#include "../debug/Log.hpp"

// Global instance definition
UP<CMoonlightManager> g_pMoonlightManager;

void CMoonlightManager::init() {
    Debug::log(LOG, "[moonlight] MoonlightManager initialized (Step 7: Input Management)");

    // Initialize core protocol infrastructure (Step 2)
    moonlight::crypto::CryptoStub::init();
    moonlight::protocol::RTSPStub::init();

    // Initialize REST API server (Step 4)
    m_restServer = std::make_unique<moonlight::rest::RestServerStub>();

    // Step 6: Initialize streaming infrastructure
    m_streamingManager = std::make_unique<moonlight::streaming::StreamingManager>();
    if (m_streamingManager->initialize()) {
        Debug::log(LOG, "[moonlight] Streaming infrastructure initialized");
    } else {
        Debug::log(ERR, "[moonlight] Failed to initialize streaming infrastructure");
    }

    // Step 7: Initialize input management
    m_inputManager = std::make_unique<moonlight::input::InputManager>();
    if (m_inputManager->initialize()) {
        Debug::log(LOG, "[moonlight] Input management initialized");
    } else {
        Debug::log(ERR, "[moonlight] Failed to initialize input management");
    }

    m_bEnabled = false; // Still disabled - just infrastructure setup
    Debug::log(LOG, "[moonlight] Core protocol, streaming, and input infrastructure ready");
}

void CMoonlightManager::destroy() {
    Debug::log(LOG, "[moonlight] MoonlightManager destroying...");

    // Step 7: Stop input manager if running
    stopInputManager();
    m_inputManager.reset();

    // Step 6: Stop streaming if running
    stopStreaming();
    m_streamingManager.reset();

    // Stop REST API if running
    stopRestAPI();
    m_restServer.reset();

    // Clean up protocol infrastructure
    moonlight::protocol::RTSPStub::destroy();
    moonlight::crypto::CryptoStub::destroy();

    m_bEnabled = false;
    Debug::log(LOG, "[moonlight] MoonlightManager destroyed");
}

bool CMoonlightManager::startRestAPI() {
    if (!m_restServer) {
        Debug::log(ERR, "[moonlight] REST server not initialized");
        return false;
    }

    if (m_restServer->isRunning()) {
        Debug::log(WARN, "[moonlight] REST API already running");
        return true;
    }

    moonlight::rest::RestConfig config;
    config.hostname = "Hyprland";
    config.http_port = 47989;
    config.https_port = 47984;
    config.uuid = "hyprland-moonlight-server";

    bool success = m_restServer->initialize(config);
    if (success) {
        Debug::log(LOG, "[moonlight] REST API started successfully");
    } else {
        Debug::log(ERR, "[moonlight] Failed to start REST API");
    }

    return success;
}

void CMoonlightManager::stopRestAPI() {
    if (m_restServer && m_restServer->isRunning()) {
        Debug::log(LOG, "[moonlight] Stopping REST API");
        m_restServer->shutdown();
    }
}

bool CMoonlightManager::isRestAPIRunning() const {
    return m_restServer && m_restServer->isRunning();
}

void CMoonlightManager::addPairedClient(const moonlight::rest::ClientInfo& client) {
    if (m_restServer) {
        m_restServer->addPairedClient(client);
    }
}

void CMoonlightManager::removePairedClient(const std::string& client_id) {
    if (m_restServer) {
        m_restServer->removePairedClient(client_id);
    }
}

std::vector<moonlight::rest::ClientInfo> CMoonlightManager::getPairedClients() const {
    if (m_restServer) {
        return m_restServer->getPairedClients();
    }
    return {};
}

// Step 6: Streaming management functions

bool CMoonlightManager::startStreaming(const std::string& client_ip, unsigned short video_port, unsigned short audio_port) {
    if (!m_streamingManager) {
        Debug::log(ERR, "[moonlight] Cannot start streaming - streaming manager not initialized");
        return false;
    }

    Debug::log(LOG, "[moonlight] Starting streaming to {}:{}(video)/{}(audio)", client_ip, video_port, audio_port);

    bool video_ok = m_streamingManager->startVideoStream(client_ip, video_port);
    bool audio_ok = m_streamingManager->startAudioStream(client_ip, audio_port);

    if (video_ok && audio_ok) {
        Debug::log(LOG, "[moonlight] Streaming started successfully");
        return true;
    } else {
        Debug::log(ERR, "[moonlight] Failed to start streaming (video: {}, audio: {})", video_ok, audio_ok);
        stopStreaming(); // Clean up partial start
        return false;
    }
}

bool CMoonlightManager::stopStreaming() {
    if (!m_streamingManager) {
        return true;
    }

    Debug::log(LOG, "[moonlight] Stopping streaming");
    bool video_ok = m_streamingManager->stopVideoStream();
    bool audio_ok = m_streamingManager->stopAudioStream();

    if (video_ok && audio_ok) {
        Debug::log(LOG, "[moonlight] Streaming stopped successfully");
    } else {
        Debug::log(WARN, "[moonlight] Issues stopping streaming (video: {}, audio: {})", video_ok, audio_ok);
    }

    return video_ok && audio_ok;
}

bool CMoonlightManager::isStreaming() const {
    return m_streamingManager &&
           (m_streamingManager->isVideoStreaming() || m_streamingManager->isAudioStreaming());
}

void CMoonlightManager::onFrameReady(CMonitor* monitor, wlr_buffer* buffer) {
    // Step 5: Basic frame capture hook with Step 6 streaming enhancement
    static int frameCount = 0;
    frameCount++;

    // Only log every 60 frames (roughly once per second at 60fps)
    if (frameCount % 60 == 0) {
        Debug::log(LOG, "[moonlight] Frame capture hook called for monitor: {} (frame #{}, streaming: {})",
                   monitor ? monitor->szName : "null", frameCount, isStreaming());
    }

    // Step 6: Forward frame to streaming manager if available
    if (m_streamingManager) {
        m_streamingManager->processFrame(monitor, buffer);
    }
}

// Step 7: Input management functions

bool CMoonlightManager::startInputManager() {
    if (!m_inputManager) {
        Debug::log(ERR, "[moonlight] Cannot start input manager - not initialized");
        return false;
    }

    if (m_inputManager->isInitialized()) {
        Debug::log(WARN, "[moonlight] Input manager already running");
        return true;
    }

    Debug::log(LOG, "[moonlight] Starting Moonlight input manager");
    return m_inputManager->initialize();
}

void CMoonlightManager::stopInputManager() {
    if (m_inputManager) {
        Debug::log(LOG, "[moonlight] Stopping Moonlight input manager");
        m_inputManager->shutdown();
    }
}

bool CMoonlightManager::isInputManagerRunning() const {
    return m_inputManager && m_inputManager->isInitialized();
}

// Input event handlers for Moonlight clients

void CMoonlightManager::handleMouseMove(float x, float y, bool relative) {
    if (m_inputManager) {
        m_inputManager->handleMouseMove(x, y, relative);
    }
}

void CMoonlightManager::handleMouseButton(int button, bool pressed) {
    if (m_inputManager) {
        m_inputManager->handleMouseButton(button, pressed);
    }
}

void CMoonlightManager::handleMouseScroll(float scrollX, float scrollY) {
    if (m_inputManager) {
        m_inputManager->handleMouseScroll(scrollX, scrollY);
    }
}

void CMoonlightManager::handleKeyboardKey(int keycode, bool pressed) {
    if (m_inputManager) {
        m_inputManager->handleKeyboardKey(keycode, pressed);
    }
}

void CMoonlightManager::handleTouchEvent(int touchId, float x, float y, bool pressed) {
    if (m_inputManager) {
        m_inputManager->handleTouchEvent(touchId, x, y, pressed);
    }
}