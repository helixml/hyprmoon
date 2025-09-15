#include "MoonlightManager.hpp"
#include "crypto/CryptoStub.hpp"
#include "protocol/RTSPStub.hpp"
#include "../debug/Log.hpp"

// Global instance definition
UP<CMoonlightManager> g_pMoonlightManager;

void CMoonlightManager::init() {
    Debug::log(LOG, "[moonlight] MoonlightManager initialized (Step 4: REST API and Pairing)");

    // Initialize core protocol infrastructure (Step 2)
    moonlight::crypto::CryptoStub::init();
    moonlight::protocol::RTSPStub::init();

    // Initialize REST API server (Step 4)
    m_restServer = std::make_unique<moonlight::rest::RestServerStub>();

    m_bEnabled = false; // Still disabled - just infrastructure setup
    Debug::log(LOG, "[moonlight] Core protocol infrastructure ready");
}

void CMoonlightManager::destroy() {
    Debug::log(LOG, "[moonlight] MoonlightManager destroying...");

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

void CMoonlightManager::onFrameReady(CMonitor* monitor, wlr_buffer* buffer) {
    // Step 5: Minimal frame capture hook - just log for now
    // This is a stub implementation to test if VNC breaks
    static int frameCount = 0;
    frameCount++;

    // Only log every 60 frames (roughly once per second at 60fps)
    if (frameCount % 60 == 0) {
        Debug::log(LOG, "[moonlight] Frame capture hook called for monitor: {} (frame #{})",
                   monitor ? monitor->szName : "null", frameCount);
    }

    // TODO Step 6: Add actual frame processing for streaming
    // For now, just ensure we don't break VNC by doing minimal work
}