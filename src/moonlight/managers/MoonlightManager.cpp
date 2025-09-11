#include "MoonlightManager.hpp"
#include "../../Compositor.hpp"
#include "../../managers/eventLoop/EventLoopManager.hpp"
#include "../../debug/Log.hpp"
#include "../../helpers/memory/Memory.hpp"

// Wolf includes will be replaced with stubs for now
// TODO: Replace with actual Wolf implementation once include paths are fixed

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

// Global instance
UP<CMoonlightManager> g_pMoonlightManager;

// Include Wolf implementation
#include "../wolf-impl/WolfMoonlightServer.hpp"

CMoonlightManager::CMoonlightManager() {
    Debug::log(LOG, "CMoonlightManager: Initializing moonlight manager");
}

CMoonlightManager::~CMoonlightManager() {
    destroy();
}

void CMoonlightManager::init() {
    if (m_initialized) {
        Debug::log(WARN, "CMoonlightManager: Already initialized");
        return;
    }
    
    Debug::log(LOG, "CMoonlightManager: Starting moonlight integration");
    
    try {
        // Load configuration
        loadConfig();
        
        // Create Wolf moonlight server
        m_wolfServer = std::make_unique<wolf::core::WolfMoonlightServer>();
        
        // Convert our config to Wolf config
        wolf::core::MoonlightConfig wolfConfig;
        wolfConfig.uuid = "hyprland-moonlight-" + std::to_string(time(nullptr));
        wolfConfig.hostname = "Hyprland";
        wolfConfig.http_port = m_config.httpPort;
        wolfConfig.https_port = m_config.httpsPort;
        wolfConfig.rtsp_port = m_config.rtspPort;
        wolfConfig.control_port = m_config.controlPort;
        wolfConfig.video_port = m_config.videoPort;
        wolfConfig.audio_port = m_config.audioPort;
        wolfConfig.video.width = 1920;
        wolfConfig.video.height = 1080;
        wolfConfig.video.fps = m_config.fps;
        wolfConfig.video.bitrate = m_config.quality;
        wolfConfig.video.encoder = m_config.encoderPreference;
        wolfConfig.audio.channels = 2;
        wolfConfig.audio.sample_rate = 48000;
        
        // Initialize Wolf server
        if (!m_wolfServer->initialize(wolfConfig)) {
            throw std::runtime_error("Failed to initialize Wolf moonlight server");
        }
        
        m_initialized = true;
        Debug::log(LOG, "CMoonlightManager: Moonlight server ready on ports HTTP:{}, HTTPS:{}, RTSP:{}", 
                  m_config.httpPort, m_config.httpsPort, m_config.rtspPort);
        
    } catch (const std::exception& e) {
        Debug::log(ERR, "CMoonlightManager: Failed to initialize: {}", e.what());
        destroy();
    }
}

void CMoonlightManager::destroy() {
    if (!m_initialized) return;
    
    Debug::log(LOG, "CMoonlightManager: Shutting down moonlight server");
    
    stopStreaming();
    
    // Cleanup Wolf server
    m_wolfServer.reset();
    
    m_initialized = false;
}

void CMoonlightManager::loadConfig() {
    // Load moonlight configuration from hyprland.conf
    // For now, use defaults
    m_config = {};
    
    Debug::log(LOG, "CMoonlightManager: Configuration loaded - Quality: {}, FPS: {}, HW Encoding: {}", 
              m_config.quality, m_config.fps, m_config.hardwareEncoding ? "enabled" : "disabled");
}

void CMoonlightManager::reloadConfig() {
    Debug::log(LOG, "CMoonlightManager: Reloading configuration");
    loadConfig();
    
    // Restart services if configuration changed significantly
    if (m_initialized) {
        // TODO: Implement smart config reload
    }
}

void CMoonlightManager::startStreaming(PHLMONITOR monitor) {
    if (!m_initialized) {
        Debug::log(ERR, "CMoonlightManager: Not initialized, cannot start streaming");
        return;
    }
    
    if (m_streaming) {
        Debug::log(WARN, "CMoonlightManager: Already streaming");
        return;
    }
    
    // Use primary monitor if none specified
    if (!monitor) {
        monitor = g_pCompositor->getMonitorFromName("");
        if (!monitor) {
            Debug::log(ERR, "CMoonlightManager: No monitor available for streaming");
            return;
        }
    }
    
    m_streamingMonitor = monitor;
    m_streaming = true;
    
    Debug::log(LOG, "CMoonlightManager: Starting stream for monitor: {}", monitor->m_name);
    
    // Start GStreamer pipeline
    if (m_pipeline) {
        gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_PLAYING);
    }
}

void CMoonlightManager::stopStreaming() {
    if (!m_streaming) return;
    
    Debug::log(LOG, "CMoonlightManager: Stopping stream");
    
    // Stop GStreamer pipeline
    if (m_pipeline) {
        gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_NULL);
    }
    
    m_streaming = false;
    m_streamingMonitor = nullptr;
}

void CMoonlightManager::onFrameReady(PHLMONITOR monitor, SP<Aquamarine::IBuffer> buffer) {
    if (!m_streaming || !buffer) return;
    
    // Only process frames for the monitor we're streaming
    if (monitor != m_streamingMonitor) return;
    
    processFrame(buffer);
}

void CMoonlightManager::processFrame(SP<Aquamarine::IBuffer> buffer) {
    // Convert Aquamarine buffer and push to Wolf moonlight server
    
    if (!m_wolfServer || !m_initialized) {
        return;
    }
    
    // Get buffer properties
    auto size = buffer->size;
    auto attrs = buffer->dmabuf();
    
    if (!attrs.success) {
        Debug::log(WARN, "CMoonlightManager: Failed to get DMA-BUF attributes");
        return;
    }
    
    // For now, we need to map the buffer to get pixel data
    // In a real implementation, we'd want to pass the DMA-BUF directly to GStreamer
    // TODO: Implement proper zero-copy DMA-BUF to GStreamer conversion
    
    // Pass frame to Wolf server (placeholder with empty data for now)
    m_wolfServer->onFrameReady(nullptr, 0, size.x, size.y, attrs.format);
}

void CMoonlightManager::setupNetworkServices() {
    Debug::log(LOG, "CMoonlightManager: Setting up network services");
    
    // Setup Wolf's HTTP/HTTPS servers for pairing
    // Setup mDNS discovery
    // Setup RTSP server for stream negotiation
    
    // TODO: Initialize Wolf's network components
}

void CMoonlightManager::setupGStreamerPipeline() {
    Debug::log(LOG, "CMoonlightManager: Setting up GStreamer pipeline");
    
    // Create custom frame source element to replace Wolf's waylanddisplaysrc
    // Setup encoding pipeline with hardware acceleration
    // Setup Wolf's custom RTP payloaders
    
    // TODO: Create HyprlandFrameSource element
    setupFrameSource();
}

void CMoonlightManager::setupInputHandling() {
    Debug::log(LOG, "CMoonlightManager: Setting up input handling");
    
    // Initialize Wolf's ENet control server
    // Setup virtual input devices via uinput
    // Connect input events to Hyprland's input system
    
    // TODO: Initialize Wolf's control server
}

void CMoonlightManager::setupFrameSource() {
    Debug::log(LOG, "CMoonlightManager: Setting up Hyprland frame source");
    
    // TODO: Create custom GStreamer element that receives frames from Hyprland
    // This replaces Wolf's waylanddisplaysrc
}

void CMoonlightManager::cleanupResources() {
    if (m_pipeline) {
        gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(m_pipeline));
        m_pipeline = nullptr;
    }
    
    m_frameSrc = nullptr;
}

void CMoonlightManager::handlePairingRequest(const std::string& clientIP) {
    Debug::log(LOG, "CMoonlightManager: Handling pairing request from {}", clientIP);
    
    // Wolf's pairing system will handle this via the REST server
    // PIN will be displayed via web interface at http://ip:47989/pin
}

void CMoonlightManager::unpairClient(const std::string& clientID) {
    Debug::log(LOG, "CMoonlightManager: Unpairing client {}", clientID);
    
    // Remove client certificate and revoke access
}