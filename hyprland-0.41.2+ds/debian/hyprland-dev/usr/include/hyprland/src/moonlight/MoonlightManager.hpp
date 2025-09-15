#pragma once
#include <memory>

class CMoonlightManager {
public:
    CMoonlightManager() = default;
    ~CMoonlightManager() = default;

    void init();
    void destroy();

private:
    bool m_bEnabled = false;
};

inline std::unique_ptr<CMoonlightManager> g_pMoonlightManager;