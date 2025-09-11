#pragma once

#include <memory>
#include <string>
#include <aquamarine/buffer/Buffer.hpp>
#include "../../helpers/Monitor.hpp"
#include "../../helpers/memory/Memory.hpp"

// Forward declarations for Wolf components
namespace wolf {
namespace core {
    class WolfMoonlightServer;
    class MoonlightConfig;
}
}

class CMoonlightManager {
public:
    CMoonlightManager();
    ~CMoonlightManager();

    void init();
    void destroy();
    
    // Streaming control
    void startStreaming(PHLMONITOR monitor = nullptr);
    void stopStreaming();
    bool isStreaming() const { return m_streaming; }
    
    // Frame callback from renderer
    void onFrameReady(PHLMONITOR monitor, SP<Aquamarine::IBuffer> buffer);
    
    // Configuration
    void loadConfig();
    void reloadConfig();
    
    // Pairing and client management
    void handlePairingRequest(const std::string& clientIP);
    void unpairClient(const std::string& clientID);
    
private:
    // Internal methods
    void setupNetworkServices();
    void setupGStreamerPipeline();
    void setupInputHandling();
    void cleanupResources();
    
    // Frame processing
    void processFrame(SP<Aquamarine::IBuffer> buffer);
    void setupFrameSource();
    
    // State
    bool m_initialized = false;
    bool m_streaming = false;
    PHLMONITOR m_streamingMonitor = nullptr;
    
    // Wolf moonlight server (using pimpl pattern to avoid header dependencies)
    std::unique_ptr<wolf::core::WolfMoonlightServer> m_wolfServer;
    
    // GStreamer pipeline
    void* m_pipeline = nullptr; // GstElement* (avoiding gstreamer headers here)
    void* m_frameSrc = nullptr; // Custom frame source element
    
    // Configuration
    struct Config {
        bool enabled = true;
        int quality = 20000; // bitrate
        int fps = 60;
        bool hardwareEncoding = true;
        std::string encoderPreference = "auto"; // nvenc, qsv, vaapi, x264
        bool audioEnabled = true;
        int httpPort = 47989;
        int httpsPort = 47984;
        int rtspPort = 48010;
        int controlPort = 47999;
        int videoPort = 48000;
        int audioPort = 48002;
    } m_config;
};

// Global instance
extern UP<CMoonlightManager> g_pMoonlightManager;