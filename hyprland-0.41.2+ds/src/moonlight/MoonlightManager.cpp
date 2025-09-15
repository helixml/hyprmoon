#include "MoonlightManager.hpp"
#include "crypto/CryptoStub.hpp"
#include "protocol/RTSPStub.hpp"
#include "../debug/Log.hpp"

void CMoonlightManager::init() {
    Debug::log(LOG, "[moonlight] MoonlightManager initialized (Step 2: Core Protocol Infrastructure)");

    // Initialize core protocol infrastructure (Step 2)
    moonlight::crypto::CryptoStub::init();
    moonlight::protocol::RTSPStub::init();

    m_bEnabled = false; // Still disabled - just infrastructure setup
    Debug::log(LOG, "[moonlight] Core protocol infrastructure ready");
}

void CMoonlightManager::destroy() {
    Debug::log(LOG, "[moonlight] MoonlightManager destroying...");

    // Clean up protocol infrastructure
    moonlight::protocol::RTSPStub::destroy();
    moonlight::crypto::CryptoStub::destroy();

    m_bEnabled = false;
    Debug::log(LOG, "[moonlight] MoonlightManager destroyed");
}