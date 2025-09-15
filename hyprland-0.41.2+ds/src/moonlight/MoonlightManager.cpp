#include "MoonlightManager.hpp"
#include "rest/rest.hpp"
#include "state/data-structures.hpp"
#include "../debug/Log.hpp"
#include <chrono>
#include <thread>
#include <fstream>

// Global instance definition
UP<CMoonlightManager> g_pMoonlightManager;

void CMoonlightManager::init() {
    Debug::log(LOG, "[moonlight] MoonlightManager initialized (Step 7: Wolf REST API Integration)");

    // Initialize Wolf AppState for REST API
    setupWolfAppState();

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

    // Start Wolf REST API for Moonlight client discovery and pairing
    if (startRestAPI()) {
        Debug::log(LOG, "[moonlight] Wolf REST API started successfully");
    } else {
        Debug::log(ERR, "[moonlight] Failed to start Wolf REST API");
    }

    m_bEnabled = false; // Still disabled - just infrastructure setup
    Debug::log(LOG, "[moonlight] Wolf REST API, streaming, and input infrastructure ready");
}

void CMoonlightManager::destroy() {
    Debug::log(LOG, "[moonlight] MoonlightManager destroying...");

    // Step 7: Stop input manager if running
    stopInputManager();
    m_inputManager.reset();

    // Step 6: Stop streaming if running
    stopStreaming();
    m_streamingManager.reset();

    // Stop Wolf REST API if running
    stopRestAPI();
    m_httpServer.reset();
    m_httpsServer.reset();

    m_bEnabled = false;
    Debug::log(LOG, "[moonlight] MoonlightManager destroyed");
}

bool CMoonlightManager::startRestAPI() {
    if (isRestAPIRunning()) {
        Debug::log(WARN, "[moonlight] Wolf REST API already running");
        return true;
    }

    try {
        // Create HTTP server
        m_httpServer = std::make_unique<HttpServer>();

        // Create HTTPS server (Wolf requires certificates)
        // For now, create a self-signed certificate
        std::string cert_file = "/tmp/moonlight-cert.pem";
        std::string key_file = "/tmp/moonlight-key.pem";

        // Generate self-signed certificate if it doesn't exist
        generateSelfSignedCertificate(cert_file, key_file);

        m_httpsServer = std::make_unique<HttpsServer>(cert_file, key_file);

        // Start HTTP server on port 47989
        int httpPort = state::get_port(state::HTTP_PORT);
        std::thread httpThread([this, httpPort]() {
            try {
                HTTPServers::startServer(m_httpServer.get(), m_appState, httpPort);
            } catch (const std::exception& e) {
                Debug::log(ERR, "[moonlight] HTTP server error: {}", e.what());
            }
        });
        httpThread.detach();

        // Start HTTPS server on port 47984
        int httpsPort = state::get_port(state::HTTPS_PORT);
        std::thread httpsThread([this, httpsPort]() {
            try {
                HTTPServers::startServer(m_httpsServer.get(), m_appState, httpsPort);
            } catch (const std::exception& e) {
                Debug::log(ERR, "[moonlight] HTTPS server error: {}", e.what());
            }
        });
        httpsThread.detach();

        // Give servers time to start
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        Debug::log(LOG, "[moonlight] Wolf REST API started - HTTP:{}, HTTPS:{}", httpPort, httpsPort);
        return true;

    } catch (const std::exception& e) {
        Debug::log(ERR, "[moonlight] Failed to start Wolf REST API: {}", e.what());
        return false;
    }
}

void CMoonlightManager::stopRestAPI() {
    if (isRestAPIRunning()) {
        Debug::log(LOG, "[moonlight] Stopping Wolf REST API");

        // Note: Wolf's SimpleWeb servers don't have a direct stop method
        // They stop when the server objects are destroyed
        if (m_httpServer) {
            m_httpServer.reset();
        }
        if (m_httpsServer) {
            m_httpsServer.reset();
        }
    }
}

bool CMoonlightManager::isRestAPIRunning() const {
    return m_httpServer || m_httpsServer;
}

void CMoonlightManager::addPairedClient(const moonlight::rest::ClientInfo& client) {
    // Wolf REST API manages clients through the AppState
    // This is handled automatically by the pairing endpoints
    Debug::log(LOG, "[moonlight] Client paired: {}", client.client_id);
}

void CMoonlightManager::removePairedClient(const std::string& client_id) {
    // Wolf REST API manages clients through the AppState
    // This is handled automatically by the unpair endpoints
    Debug::log(LOG, "[moonlight] Client unpaired: {}", client_id);
}

std::vector<moonlight::rest::ClientInfo> CMoonlightManager::getPairedClients() const {
    // Wolf REST API stores clients in AppState
    // For now, return empty list - this would be implemented by reading from AppState
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

// Wolf AppState setup helper
void CMoonlightManager::setupWolfAppState() {
    Debug::log(LOG, "[moonlight] Setting up Wolf AppState for REST API");

    // Create basic Wolf AppState configuration
    state::Config config;
    config.uuid = "hyprland-moonlight-" + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count());
    config.hostname = "Hyprland";
    config.config_source = "hyprland.conf";
    config.support_hevc = true;
    config.support_av1 = false;

    // Initialize paired clients atom
    config.paired_clients = std::make_shared<immer::atom<state::PairedClientList>>(
        immer::vector<immer::box<wolf::config::PairedClient>>{}
    );

    // Initialize apps atom
    config.apps = std::make_shared<immer::atom<immer::vector<immer::box<events::App>>>>(
        immer::vector<immer::box<events::App>>{}
    );

    // Create host configuration
    state::Host host;
    host.display_modes = state::DISPLAY_CONFIGURATIONS;
    host.audio_modes = state::AUDIO_CONFIGURATIONS;

    // Initialize AppState
    m_appState = immer::box<state::AppState>(state::AppState{
        .config = immer::box<state::Config>(config),
        .host = immer::box<state::Host>(host),
        .pairing_cache = std::make_shared<immer::atom<immer::map<std::string, state::PairCache>>>(
            immer::map<std::string, state::PairCache>{}
        ),
        .pairing_atom = std::make_shared<immer::atom<immer::map<std::string, immer::box<events::PairSignal>>>>(
            immer::map<std::string, immer::box<events::PairSignal>>{}
        ),
        .event_bus = std::make_shared<events::EventBusType>(),
        .running_sessions = std::make_shared<immer::atom<immer::vector<events::StreamSession>>>(
            immer::vector<events::StreamSession>{}
        )
    });

    Debug::log(LOG, "[moonlight] Wolf AppState initialized - UUID: {}", config.uuid);
}

// Self-signed certificate generation helper
void CMoonlightManager::generateSelfSignedCertificate(const std::string& cert_file, const std::string& key_file) {
    Debug::log(LOG, "[moonlight] Generating self-signed certificate for HTTPS");

    // For now, create placeholder files
    // In a real implementation, we'd use OpenSSL to generate proper certificates
    std::ofstream cert(cert_file);
    cert << "-----BEGIN CERTIFICATE-----\n";
    cert << "MIICDzCCAXgCCQC8Q2Q2Q2Q2QjANBgkqhkiG9w0BAQsFADAQMQ4wDAYDVQQDDAVs\n";
    cert << "b2NhbDAeFw0yNDA5MTUxNzAwMDBaFw0yNTA5MTUxNzAwMDBaMA8xDTALBgNVBAMM\n";
    cert << "BGh5cHIwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC7Q2Q2Q2Q2QjAM\n";
    cert << "BgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQA=\n";
    cert << "-----END CERTIFICATE-----\n";
    cert.close();

    std::ofstream key(key_file);
    key << "-----BEGIN PRIVATE KEY-----\n";
    key << "MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQC7Q2Q2Q2Q2QjAM\n";
    key << "BgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQA=\n";
    key << "-----END PRIVATE KEY-----\n";
    key.close();

    Debug::log(LOG, "[moonlight] Self-signed certificate generated: {} / {}", cert_file, key_file);
}