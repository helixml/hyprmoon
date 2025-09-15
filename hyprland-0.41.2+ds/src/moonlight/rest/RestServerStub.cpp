#include "RestServerStub.hpp"
#include "../../debug/Log.hpp"
#include <chrono>
#include <mutex>

namespace moonlight {
namespace rest {

RestServerStub::RestServerStub() {
    Debug::log(LOG, "[moonlight] Creating RestServerStub (Step 4: REST API and Pairing)");
}

RestServerStub::~RestServerStub() {
    shutdown();
}

bool RestServerStub::initialize(const RestConfig& config) {
    if (m_initialized) {
        Debug::log(WARN, "[moonlight] RestServerStub already initialized");
        return true;
    }

    m_config = config;

    Debug::log(LOG, "[moonlight] RestServerStub: Initializing REST API server");
    Debug::log(LOG, "[moonlight] RestServerStub: HTTP port: {}, HTTPS port: {}",
               m_config.http_port, m_config.https_port);
    Debug::log(LOG, "[moonlight] RestServerStub: Hostname: {}", m_config.hostname);

    try {
        // Start HTTP server thread (stub implementation)
        m_httpThread = std::make_unique<std::thread>(&RestServerStub::httpServerThread, this);

        // Start HTTPS server thread (stub implementation)
        m_httpsThread = std::make_unique<std::thread>(&RestServerStub::httpsServerThread, this);

        m_running = true;
        m_initialized = true;

        Debug::log(LOG, "[moonlight] RestServerStub: Successfully started REST API servers");
        Debug::log(LOG, "[moonlight] RestServerStub: Moonlight clients can connect to:");
        Debug::log(LOG, "[moonlight] RestServerStub:   HTTP:  http://{}:{}", m_config.hostname, m_config.http_port);
        Debug::log(LOG, "[moonlight] RestServerStub:   HTTPS: https://{}:{}", m_config.hostname, m_config.https_port);

        return true;
    } catch (const std::exception& e) {
        Debug::log(ERR, "[moonlight] RestServerStub: Failed to initialize: {}", e.what());
        return false;
    }
}

void RestServerStub::shutdown() {
    if (!m_running) return;

    Debug::log(LOG, "[moonlight] RestServerStub: Shutting down REST API servers");
    m_running = false;

    if (m_httpThread && m_httpThread->joinable()) {
        m_httpThread->join();
    }
    if (m_httpsThread && m_httpsThread->joinable()) {
        m_httpsThread->join();
    }

    m_httpThread.reset();
    m_httpsThread.reset();
    m_initialized = false;

    Debug::log(LOG, "[moonlight] RestServerStub: REST API servers stopped");
}

void RestServerStub::addPairedClient(const ClientInfo& client) {
    std::lock_guard<std::mutex> lock(m_clientsMutex);

    // Remove existing client with same ID
    m_pairedClients.erase(
        std::remove_if(m_pairedClients.begin(), m_pairedClients.end(),
                      [&](const ClientInfo& c) { return c.client_id == client.client_id; }),
        m_pairedClients.end());

    // Add new client
    m_pairedClients.push_back(client);

    Debug::log(LOG, "[moonlight] RestServerStub: Added paired client: {} ({})",
               client.client_name, client.client_id);
}

void RestServerStub::removePairedClient(const std::string& client_id) {
    std::lock_guard<std::mutex> lock(m_clientsMutex);

    auto it = std::remove_if(m_pairedClients.begin(), m_pairedClients.end(),
                            [&](const ClientInfo& c) { return c.client_id == client_id; });

    if (it != m_pairedClients.end()) {
        Debug::log(LOG, "[moonlight] RestServerStub: Removed paired client: {}", client_id);
        m_pairedClients.erase(it, m_pairedClients.end());
    }
}

std::vector<ClientInfo> RestServerStub::getPairedClients() const {
    std::lock_guard<std::mutex> lock(m_clientsMutex);
    return m_pairedClients;
}

void RestServerStub::httpServerThread() {
    Debug::log(LOG, "[moonlight] RestServerStub: HTTP server thread started on port {} (stub implementation)",
               m_config.http_port);

    // Stub implementation - just sleep and log periodically
    auto lastLogTime = std::chrono::steady_clock::now();
    const auto LOG_INTERVAL = std::chrono::minutes(1);

    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto now = std::chrono::steady_clock::now();
        if (now - lastLogTime >= LOG_INTERVAL) {
            Debug::log(LOG, "[moonlight] RestServerStub: HTTP server active on port {} ({})",
                       m_config.http_port, m_pairedClients.size());
            lastLogTime = now;
        }
    }

    Debug::log(LOG, "[moonlight] RestServerStub: HTTP server thread finished");
}

void RestServerStub::httpsServerThread() {
    Debug::log(LOG, "[moonlight] RestServerStub: HTTPS server thread started on port {} (stub implementation)",
               m_config.https_port);

    // Stub implementation - just sleep and log periodically
    auto lastLogTime = std::chrono::steady_clock::now();
    const auto LOG_INTERVAL = std::chrono::minutes(1);

    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto now = std::chrono::steady_clock::now();
        if (now - lastLogTime >= LOG_INTERVAL) {
            Debug::log(LOG, "[moonlight] RestServerStub: HTTPS server active on port {} ({} paired clients)",
                       m_config.https_port, m_pairedClients.size());
            lastLogTime = now;
        }
    }

    Debug::log(LOG, "[moonlight] RestServerStub: HTTPS server thread finished");
}

} // namespace rest
} // namespace moonlight