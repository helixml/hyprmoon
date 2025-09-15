#pragma once
#include "../helpers/memory/Memory.hpp"

class CMoonlightManager {
public:
    CMoonlightManager() = default;
    ~CMoonlightManager() = default;

    void init();
    void destroy();

private:
    bool m_bEnabled = false;
};

// Global instance
extern UP<CMoonlightManager> g_pMoonlightManager;