#pragma once
#include "../helpers/memory/Memory.hpp"
#include "rest/RestServerStub.hpp"
#include <memory>

class CMoonlightManager {
public:
    CMoonlightManager() = default;
    ~CMoonlightManager() = default;

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

    // Frame capture (Step 5) - following previous implementation pattern
    void onFrameReady(class CMonitor* monitor, struct wlr_buffer* buffer);

private:
    bool m_bEnabled = false;

    // REST API server (Wolf-compatible)
    std::unique_ptr<moonlight::rest::RestServerStub> m_restServer;
};

// Global instance
extern UP<CMoonlightManager> g_pMoonlightManager;