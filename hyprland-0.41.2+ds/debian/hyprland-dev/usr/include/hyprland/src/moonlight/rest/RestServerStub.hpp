#pragma once

#include <memory>
#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include <map>

// Step 4: Minimal REST API stub - Wolf-compatible interface without heavy dependencies
// This will be expanded in later steps to use the full Wolf implementation

namespace moonlight {
namespace rest {

// Basic configuration for Step 4
struct RestConfig {
    int http_port = 47989;
    int https_port = 47984;
    std::string hostname = "Hyprland";
    std::string uuid = "hyprland-moonlight-server";
};

// Basic client info for pairing
struct ClientInfo {
    std::string client_id;
    std::string client_name;
    std::string client_cert;
};

// Minimal REST server stub
class RestServerStub {
public:
    RestServerStub();
    ~RestServerStub();

    bool initialize(const RestConfig& config = RestConfig{});
    void shutdown();

    bool isRunning() const { return m_running; }
    int getHttpPort() const { return m_config.http_port; }
    int getHttpsPort() const { return m_config.https_port; }

    // Client management stubs
    void addPairedClient(const ClientInfo& client);
    void removePairedClient(const std::string& client_id);
    std::vector<ClientInfo> getPairedClients() const;

private:
    void httpServerThread();
    void httpsServerThread();

    RestConfig m_config;
    std::atomic<bool> m_running{false};
    std::atomic<bool> m_initialized{false};

    // Server threads
    std::unique_ptr<std::thread> m_httpThread;
    std::unique_ptr<std::thread> m_httpsThread;

    // Client storage (stub)
    std::vector<ClientInfo> m_pairedClients;
    mutable std::mutex m_clientsMutex;
};

} // namespace rest
} // namespace moonlight