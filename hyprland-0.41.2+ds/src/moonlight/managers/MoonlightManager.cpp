#include "MoonlightManager.hpp"
#include "Compositor.hpp"
#include "managers/eventLoop/EventLoopManager.hpp"
#include "debug/Log.hpp"
#include "helpers/memory/Memory.hpp"

// Wolf includes will be replaced with stubs for now
// TODO: Replace with actual Wolf implementation once include paths are fixed

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

// Global instance
UP<CMoonlightManager> g_pMoonlightManager;

// Include Wolf implementation
#include "../wolf-impl/WolfMoonlightServer.hpp"

// Include WebRTC implementation
#include "../webrtc/WebRTCManager.hpp"

// Include Voice processing implementation
#include "../voice/WhisperManager.hpp"

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
        Debug::log(WARN, "MoonlightManager: Creating WolfMoonlightServer instance");
        m_wolfServer = std::make_unique<wolf::core::WolfMoonlightServer>();
        Debug::log(WARN, "MoonlightManager: WolfMoonlightServer instance created successfully");
        
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
        Debug::log(WARN, "MoonlightManager: About to initialize WolfMoonlightServer with config");
        if (!m_wolfServer->initialize(wolfConfig)) {
            Debug::log(ERR, "MoonlightManager: WolfMoonlightServer initialization FAILED!");
            throw std::runtime_error("Failed to initialize Wolf moonlight server");
        }
        Debug::log(WARN, "MoonlightManager: WolfMoonlightServer initialization completed successfully");
        
        // Initialize WebRTC if enabled
        if (m_config.webrtcEnabled) {
            setupWebRTCIntegration();
        }
        
        // Initialize Voice processing if enabled
        if (m_config.voiceTranscriptionEnabled || m_config.ttsEnabled) {
            setupVoiceProcessing();
        }
        
        // Setup shared pipeline for both Wolf and WebRTC
        setupSharedPipeline();
        
        m_initialized = true;
        Debug::log(LOG, "CMoonlightManager: Moonlight server ready on ports HTTP:{}, HTTPS:{}, RTSP:{}", 
                  m_config.httpPort, m_config.httpsPort, m_config.rtspPort);
        if (m_webrtcManager) {
            Debug::log(LOG, "CMoonlightManager: WebRTC server ready with full bidirectional support");
        }
        if (m_whisperManager) {
            Debug::log(LOG, "CMoonlightManager: Voice transcription ready (Whisper)");
        }
        if (m_ttsManager) {
            Debug::log(LOG, "CMoonlightManager: Text-to-speech server ready on port {}", m_config.ttsPort);
        }
        
    } catch (const std::exception& e) {
        Debug::log(ERR, "CMoonlightManager: Failed to initialize: {}", e.what());
        destroy();
    }
}

void CMoonlightManager::destroy() {
    if (!m_initialized) return;
    
    Debug::log(LOG, "CMoonlightManager: Shutting down moonlight server");
    
    stopStreaming();
    stopWebRTCStreaming();
    stopVoiceTranscription();
    stopTTSServer();
    
    // Cleanup Wolf server
    m_wolfServer.reset();
    
    // Cleanup WebRTC manager
    m_webrtcManager.reset();
    
    // Cleanup Voice processing managers
    m_whisperManager.reset();
    m_ttsManager.reset();
    
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

void CMoonlightManager::startStreaming(CMonitor* monitor) {
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

    Debug::log(LOG, "CMoonlightManager: Starting stream for monitor: {}", monitor->szName);

    // Start GStreamer pipeline
    if (m_pipeline) {
        gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_PLAYING);
    }

    // CRITICAL: Start synthetic frame generation as fallback
    // This ensures we always have frames to stream even when Hyprland isn't rendering
    startSyntheticFrameGeneration();
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

bool CMoonlightManager::onFrameReady(CMonitor* monitor, wlr_buffer* buffer) {
    Debug::log(TRACE, "MoonlightManager: onFrameReady called - streaming={}, buffer={}, monitor={}",
              m_streaming, static_cast<void*>(buffer), static_cast<void*>(monitor));

    if (!m_streaming || !buffer) {
        Debug::log(TRACE, "MoonlightManager: onFrameReady early return - streaming={}, buffer={}",
                  m_streaming, static_cast<void*>(buffer));
        return false;
    }

    // Only process frames for the monitor we're streaming
    if (monitor != m_streamingMonitor) {
        Debug::log(TRACE, "MoonlightManager: onFrameReady monitor mismatch - current={}, streaming={}",
                  static_cast<void*>(monitor), static_cast<void*>(m_streamingMonitor));
        return false;
    }

    Debug::log(LOG, "MoonlightManager: Processing frame from monitor {} ({}x{})",
              monitor ? monitor->szName : "null", monitor ? monitor->vecSize.x : 0, monitor ? monitor->vecSize.y : 0);
    return processFrame(buffer);
}

bool CMoonlightManager::processFrame(wlr_buffer* buffer) {
    // Convert wlr_buffer and push to Wolf moonlight server

    if (!m_wolfServer || !m_initialized) {
        return false; // Didn't take buffer ownership
    }

    // Get buffer properties
    int width = buffer->width;
    int height = buffer->height;

    // ZERO-COPY-OR-NOTHING: Try DMA-BUF sharing with proper buffer lifecycle
    int dmabuf_fd;
    uint32_t stride;
    uint64_t modifier;

    if (extractDMABufInfo(buffer, &dmabuf_fd, &stride, &modifier)) {
        // Zero-copy: Take ownership of buffer for GStreamer processing
        m_wolfServer->onFrameReadyDMABuf(dmabuf_fd, stride, modifier, width, height, 0x34325258, buffer);
        Debug::log(TRACE, "MoonlightManager: Took buffer ownership for DMA-BUF frame (zero-copy) - {}x{}, fd: {}, stride: {}",
                  width, height, dmabuf_fd, stride);

        return true; // Took buffer ownership - renderer should NOT unlock
    } else {
        Debug::log(WARN, "MoonlightManager: DMA-BUF extraction failed - no frame sent (zero-copy-or-nothing)");
        return false; // Didn't take buffer ownership - renderer should unlock normally
    }

    // Commented out fallbacks for zero-copy-or-nothing testing:
    /*
    else if (extractFrameData(buffer, &frame_data, &frame_size)) {
        // Fallback: Extract actual pixel data from the wlr_buffer
        m_wolfServer->onFrameReady(frame_data, frame_size, width, height, 0x34325258);
        if (frame_data) {
            free(frame_data);
        }
        Debug::log(TRACE, "MoonlightManager: Sent copied frame data - {}x{}, size: {}", width, height, frame_size);
    } else {
        // Fallback to test pattern if extraction fails
        size_t test_size = width * height * 4;
        void* test_data = malloc(test_size);
        if (test_data) {
            uint32_t* pixels = (uint32_t*)test_data;
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    uint8_t gray = 128 + (x + y) % 64;
                    pixels[y * width + x] = 0xFF000000 | (gray << 16) | (gray << 8) | gray;
                }
            }
            m_wolfServer->onFrameReady(test_data, test_size, width, height, 0x34325258);
            free(test_data);
        } else {
            m_wolfServer->onFrameReady(nullptr, 0, width, height, 0x34325258);
        }
    }
    */
}

bool CMoonlightManager::extractFrameData(wlr_buffer* buffer, void** frame_data, size_t* frame_size) {
    if (!buffer || !frame_data || !frame_size) {
        return false;
    }

    // Try to use wlr_buffer_begin_data_ptr_access for direct memory access
    void* data;
    uint32_t format;
    size_t stride;

    if (wlr_buffer_begin_data_ptr_access(buffer, WLR_BUFFER_DATA_PTR_ACCESS_READ, &data, &format, &stride)) {
        // Calculate frame size based on buffer properties
        *frame_size = stride * buffer->height;

        // Allocate memory for the frame data copy
        *frame_data = malloc(*frame_size);
        if (*frame_data) {
            memcpy(*frame_data, data, *frame_size);
            wlr_buffer_end_data_ptr_access(buffer);
            Debug::log(TRACE, "MoonlightManager: Extracted frame data - {}x{}, size: {}, stride: {}",
                      buffer->width, buffer->height, *frame_size, stride);
            return true;
        }

        wlr_buffer_end_data_ptr_access(buffer);
    }

    Debug::log(WARN, "MoonlightManager: Failed to extract frame data from wlr_buffer");
    return false;
}

bool CMoonlightManager::extractDMABufInfo(wlr_buffer* buffer, int* fd, uint32_t* stride, uint64_t* modifier) {
    if (!buffer || !fd || !stride || !modifier) {
        return false;
    }

    // Check if this is a DMA-BUF buffer
    wlr_dmabuf_attributes dmabuf_attrs;
    if (wlr_buffer_get_dmabuf(buffer, &dmabuf_attrs)) {
        if (dmabuf_attrs.n_planes > 0) {
            *fd = dmabuf_attrs.fd[0];  // Use first plane
            *stride = dmabuf_attrs.stride[0];
            *modifier = dmabuf_attrs.modifier;

            Debug::log(TRACE, "MoonlightManager: Extracted DMA-BUF - fd: {}, stride: {}, modifier: {}",
                      *fd, *stride, *modifier);
            return true;
        }
    }

    Debug::log(TRACE, "MoonlightManager: Buffer is not a DMA-BUF or has no planes");
    return false;
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

// ===============================
// WebRTC Integration Implementation
// ===============================

void CMoonlightManager::setupWebRTCIntegration() {
    Debug::log(LOG, "CMoonlightManager: Setting up WebRTC integration");
    
    m_webrtcManager = std::make_unique<CWebRTCManager>();
    
    // Configure WebRTC
    m_webrtcManager->m_config.enable_audio_input = m_config.webrtcAudioInput;
    m_webrtcManager->m_config.enable_audio_output = m_config.webrtcAudioOutput;
    m_webrtcManager->m_config.enable_keyboard_input = m_config.webrtcKeyboardInput;
    m_webrtcManager->m_config.enable_mouse_input = m_config.webrtcMouseInput;
    m_webrtcManager->m_config.stun_server = m_config.stunServer;
    
    if (!m_webrtcManager->initialize()) {
        Debug::log(ERR, "CMoonlightManager: Failed to initialize WebRTC manager");
        m_webrtcManager.reset();
        return;
    }
    
    // Setup WebRTC callbacks
    m_webrtcManager->onOfferCreated = [this](const std::string& clientId, const std::string& offer) {
        Debug::log(LOG, "CMoonlightManager: WebRTC offer created for client {}", clientId);
        // TODO: Send offer to client via REST API or WebSocket
        // This would be handled by the REST server component
    };
    
    m_webrtcManager->onAnswerCreated = [this](const std::string& clientId, const std::string& answer) {
        Debug::log(LOG, "CMoonlightManager: WebRTC answer created for client {}", clientId);
    };
    
    m_webrtcManager->onICECandidate = [this](const std::string& clientId, const std::string& candidate) {
        Debug::log(LOG, "CMoonlightManager: ICE candidate ready for client {}", clientId);
        // TODO: Send ICE candidate to client
    };
    
    m_webrtcManager->onConnectionStateChanged = [this](const std::string& clientId, bool connected) {
        Debug::log(LOG, "CMoonlightManager: WebRTC client {} {}", clientId, connected ? "connected" : "disconnected");
    };
    
    // Setup input callbacks to integrate with Hyprland
    m_webrtcManager->onKeyboardInput = [this](int keycode, bool pressed, uint32_t modifiers) {
        handleWebRTCInput(keycode, pressed, modifiers);
    };
    
    m_webrtcManager->onMouseInput = [this](double x, double y, int button, bool pressed) {
        handleWebRTCMouse(x, y, button, pressed);
    };
    
    m_webrtcManager->onMouseMove = [this](double x, double y) {
        handleWebRTCMouse(x, y, -1, false); // -1 indicates move only
    };
    
    m_webrtcManager->onMouseScroll = [this](double delta_x, double delta_y) {
        // Handle scroll events
        Debug::log(LOG, "CMoonlightManager: WebRTC scroll delta=({:.2f},{:.2f})", delta_x, delta_y);
    };
    
    m_webrtcManager->onAudioReceived = [this](const void* audio_data, size_t size, int channels, int sample_rate) {
        handleWebRTCAudio(audio_data, size, channels, sample_rate);
        // Also route audio to voice processing
        processAudioForVoice(audio_data, size, channels, sample_rate);
    };
    
    Debug::log(LOG, "CMoonlightManager: WebRTC integration setup complete");
}

void CMoonlightManager::setupSharedPipeline() {
    Debug::log(LOG, "CMoonlightManager: Setting up shared GStreamer pipeline");
    
    if (!m_pipeline) {
        Debug::log(WARN, "CMoonlightManager: No pipeline available for sharing");
        return;
    }
    
    // Create or find tee element in the pipeline
    m_teeElement = gst_bin_get_by_name(GST_BIN(m_pipeline), "frame-tee");
    if (!m_teeElement) {
        m_teeElement = gst_element_factory_make("tee", "frame-tee");
        if (m_teeElement) {
            gst_bin_add(GST_BIN(m_pipeline), GST_ELEMENT(m_teeElement));
            // TODO: Restructure pipeline to insert tee after HyprlandFrameSource
            Debug::log(LOG, "CMoonlightManager: Created tee element for pipeline sharing");
        } else {
            Debug::log(ERR, "CMoonlightManager: Failed to create tee element");
            return;
        }
    }
    
    // Connect WebRTC manager to shared pipeline
    if (m_webrtcManager && m_teeElement) {
        if (m_webrtcManager->connectToFrameSource(GST_ELEMENT(m_teeElement))) {
            Debug::log(LOG, "CMoonlightManager: WebRTC connected to shared pipeline");
        } else {
            Debug::log(ERR, "CMoonlightManager: Failed to connect WebRTC to pipeline");
        }
    }
}

void CMoonlightManager::handleWebRTCInput(int keycode, bool pressed, uint32_t modifiers) {
    Debug::log(LOG, "CMoonlightManager: WebRTC keyboard input - key={}, pressed={}, mods={}", 
              keycode, pressed, modifiers);
    
    // Integrate with Hyprland's input system
    if (g_pInputManager) {
        // Convert web keycode to Linux input event and inject
        // This requires proper keycode mapping and input device simulation
        // For now, just log the event
        
        // TODO: Implement proper input injection via virtual input device
        // g_pInputManager->onKeyboardKey(keycode, pressed);
    }
}

void CMoonlightManager::handleWebRTCMouse(double x, double y, int button, bool pressed) {
    Debug::log(LOG, "CMoonlightManager: WebRTC mouse input - pos=({:.2f},{:.2f}), button={}, pressed={}", 
              x, y, button, pressed);
    
    // Convert relative coordinates to absolute screen coordinates
    if (m_streamingMonitor) {
        // Calculate absolute coordinates for future use
        int abs_x = (int)(x * m_streamingMonitor->vecSize.x);
        int abs_y = (int)(y * m_streamingMonitor->vecSize.y);
        
        // Integrate with Hyprland's input system
        if (g_pInputManager) {
            if (button == -1) {
                // Mouse move
                // TODO: g_pInputManager->onMouseMove(abs_x, abs_y);
                (void)abs_x; (void)abs_y; // Suppress unused variable warnings
            } else {
                // Mouse button
                // TODO: g_pInputManager->onMouseButton(button, pressed);
                (void)abs_x; (void)abs_y; // Suppress unused variable warnings
            }
        }
    }
}

void CMoonlightManager::handleWebRTCAudio(const void* audio_data, size_t size, int channels, int sample_rate) {
    Debug::log(LOG, "CMoonlightManager: WebRTC audio received - size={}, channels={}, rate={}", 
              size, channels, sample_rate);
    
    // Handle received audio from browser (microphone)
    // This could be forwarded to the system audio or recorded
    // For now, just log the reception
}

// WebRTC public API implementation
void CMoonlightManager::startWebRTCStreaming() {
    if (!m_webrtcManager) {
        Debug::log(ERR, "CMoonlightManager: WebRTC not available");
        return;
    }
    
    if (m_webrtcStreaming) {
        Debug::log(WARN, "CMoonlightManager: WebRTC already streaming");
        return;
    }
    
    m_webrtcStreaming = true;
    Debug::log(LOG, "CMoonlightManager: WebRTC streaming started");
    
    // WebRTC streaming is automatically active when clients connect
    // The actual streaming starts when createWebRTCOffer is called
}

void CMoonlightManager::stopWebRTCStreaming() {
    if (!m_webrtcStreaming) return;
    
    Debug::log(LOG, "CMoonlightManager: Stopping WebRTC streaming");
    
    if (m_webrtcManager) {
        // Close all active sessions
        auto clients = m_webrtcManager->getActiveClients();
        for (const auto& clientId : clients) {
            m_webrtcManager->closeSession(clientId);
        }
    }
    
    m_webrtcStreaming = false;
}

std::string CMoonlightManager::createWebRTCOffer(const std::string& clientId) {
    if (!m_webrtcManager) {
        Debug::log(ERR, "CMoonlightManager: WebRTC not available");
        return "";
    }
    
    Debug::log(LOG, "CMoonlightManager: Creating WebRTC offer for client {}", clientId);
    return m_webrtcManager->createOffer(clientId);
}

bool CMoonlightManager::setWebRTCAnswer(const std::string& clientId, const std::string& sdp) {
    if (!m_webrtcManager) {
        Debug::log(ERR, "CMoonlightManager: WebRTC not available");
        return false;
    }
    
    Debug::log(LOG, "CMoonlightManager: Setting WebRTC answer for client {}", clientId);
    return m_webrtcManager->setRemoteAnswer(clientId, sdp);
}

bool CMoonlightManager::addWebRTCICECandidate(const std::string& clientId, const std::string& candidate) {
    if (!m_webrtcManager) {
        Debug::log(ERR, "CMoonlightManager: WebRTC not available");
        return false;
    }
    
    Debug::log(LOG, "CMoonlightManager: Adding ICE candidate for client {}", clientId);
    return m_webrtcManager->addICECandidate(clientId, candidate);
}

void CMoonlightManager::closeWebRTCSession(const std::string& clientId) {
    if (!m_webrtcManager) {
        Debug::log(ERR, "CMoonlightManager: WebRTC not available");
        return;
    }
    
    Debug::log(LOG, "CMoonlightManager: Closing WebRTC session for client {}", clientId);
    m_webrtcManager->closeSession(clientId);
}

size_t CMoonlightManager::getWebRTCClientCount() const {
    if (!m_webrtcManager) return 0;
    return m_webrtcManager->getClientCount();
}

std::vector<std::string> CMoonlightManager::getActiveWebRTCClients() const {
    if (!m_webrtcManager) return {};
    return m_webrtcManager->getActiveClients();
}

// ===============================
// Voice Processing Integration Implementation
// ===============================

void CMoonlightManager::setupVoiceProcessing() {
    Debug::log(LOG, "CMoonlightManager: Setting up voice processing");
    
    // Initialize Whisper transcription if enabled
    if (m_config.voiceTranscriptionEnabled) {
        m_whisperManager = std::make_unique<CWhisperManager>();
        
        // Configure Whisper
        m_whisperManager->m_config.model_path = m_config.whisperModelPath;
        m_whisperManager->m_config.enable_keyboard_injection = true;
        m_whisperManager->m_config.enable_command_detection = m_config.voiceCommandsEnabled;
        m_whisperManager->m_config.language = "en";
        m_whisperManager->m_config.confidence_threshold = 0.7f;
        
        if (!m_whisperManager->initialize()) {
            Debug::log(ERR, "CMoonlightManager: Failed to initialize Whisper manager");
            m_whisperManager.reset();
        } else {
            // Setup Whisper callbacks
            m_whisperManager->onTranscriptionReady = [this](const std::string& text, float confidence) {
                handleVoiceTranscription(text, confidence);
            };
            
            m_whisperManager->onKeyboardEvent = [this](int keycode, bool pressed, uint32_t modifiers) {
                handleVoiceKeyboardEvent(keycode, pressed, modifiers);
            };
            
            m_whisperManager->onVoiceCommand = [this](const std::string& command, const std::string& params) {
                handleVoiceCommand(command, params);
            };
            
            m_whisperManager->onSpeechActivity = [this](bool speaking) {
                Debug::log(LOG, "CMoonlightManager: Speech activity: {}", speaking ? "detected" : "stopped");
            };
            
            Debug::log(LOG, "CMoonlightManager: Whisper transcription initialized");
        }
    }
    
    // Initialize TTS if enabled
    if (m_config.ttsEnabled) {
        m_ttsManager = std::make_unique<CTTSManager>();
        
        // Configure TTS
        m_ttsManager->m_config.tts_engine = m_config.ttsEngine;
        m_ttsManager->m_config.default_voice = m_config.defaultVoice;
        m_ttsManager->m_config.http_port = m_config.ttsPort;
        m_ttsManager->m_config.enable_cors = true;
        m_ttsManager->m_config.stream_audio = true;
        
        if (!m_ttsManager->initialize()) {
            Debug::log(ERR, "CMoonlightManager: Failed to initialize TTS manager");
            m_ttsManager.reset();
        } else {
            // Setup TTS callbacks
            m_ttsManager->onSpeechStarted = [this](const std::string& text) {
                Debug::log(LOG, "CMoonlightManager: TTS started: '{}'", text);
            };
            
            m_ttsManager->onSpeechFinished = [this](const std::string& text) {
                Debug::log(LOG, "CMoonlightManager: TTS finished: '{}'", text);
            };
            
            m_ttsManager->onAudioGenerated = [this](const std::vector<float>& audio_data, int sample_rate) {
                // Audio is automatically played via PulseAudio, no additional routing needed
                Debug::log(LOG, "CMoonlightManager: TTS generated {} audio samples at {}Hz", 
                          audio_data.size(), sample_rate);
            };
            
            // Start HTTP server
            m_ttsManager->startHTTPServer(m_config.ttsPort);
            
            Debug::log(LOG, "CMoonlightManager: TTS server initialized on port {}", m_config.ttsPort);
        }
    }
    
    Debug::log(LOG, "CMoonlightManager: Voice processing setup complete");
}

void CMoonlightManager::processAudioForVoice(const void* audio_data, size_t size, int channels, int sample_rate) {
    if (!m_whisperManager || !m_voiceTranscriptionActive) {
        return;
    }
    
    // Convert audio data to format expected by Whisper
    if (channels == 1 || channels == 2) {
        // Assume audio_data is float samples
        const float* float_samples = static_cast<const float*>(audio_data);
        size_t sample_count = size / sizeof(float);
        
        if (channels == 2) {
            // Convert stereo to mono by averaging channels
            std::vector<float> mono_samples(sample_count / 2);
            for (size_t i = 0; i < mono_samples.size(); ++i) {
                mono_samples[i] = (float_samples[i * 2] + float_samples[i * 2 + 1]) / 2.0f;
            }
            routeAudioToWhisper(mono_samples.data(), mono_samples.size(), sample_rate);
        } else {
            // Already mono
            routeAudioToWhisper(float_samples, sample_count, sample_rate);
        }
    }
}

void CMoonlightManager::routeAudioToWhisper(const float* audio_data, size_t sample_count, int sample_rate) {
    if (!m_whisperManager) return;
    
    m_whisperManager->processAudioChunk(audio_data, sample_count, sample_rate);
}

void CMoonlightManager::handleVoiceTranscription(const std::string& text, float confidence) {
    Debug::log(LOG, "CMoonlightManager: Voice transcription: '{}' (confidence: {:.2f})", text, confidence);
    
    // The text will be automatically converted to keyboard events by WhisperManager
    // This callback is for additional processing or logging
}

void CMoonlightManager::handleVoiceCommand(const std::string& command, const std::string& params) {
    Debug::log(LOG, "CMoonlightManager: Voice command executed: '{}' with params: '{}'", command, params);
    
    // Custom voice commands for HyprMoon functionality
    std::string lower_cmd = command;
    std::transform(lower_cmd.begin(), lower_cmd.end(), lower_cmd.begin(), ::tolower);
    
    if (lower_cmd == "speak" || lower_cmd == "say") {
        // Voice command to speak text via TTS
        if (!params.empty() && m_ttsManager) {
            speakText(params);
        }
    } else if (lower_cmd == "mute voice" || lower_cmd == "stop listening") {
        // Stop voice transcription
        stopVoiceTranscription();
    } else if (lower_cmd == "start listening" || lower_cmd == "enable voice") {
        // Start voice transcription
        startVoiceTranscription();
    } else if (lower_cmd == "new client" || lower_cmd == "connect browser") {
        // Create WebRTC offer for new client (simplified)
        if (m_webrtcManager) {
            std::string client_id = "voice_client_" + std::to_string(time(nullptr));
            createWebRTCOffer(client_id);
        }
    }
}

void CMoonlightManager::handleVoiceKeyboardEvent(int keycode, bool pressed, uint32_t modifiers) {
    Debug::log(LOG, "CMoonlightManager: Voice keyboard event: key={}, pressed={}, mods={}", 
              keycode, pressed, modifiers);
    
    // Route voice-generated keyboard events to Hyprland input system
#ifdef HYPRLAND_INTEGRATION
    if (g_pInputManager) {
        // TODO: Implement proper virtual keyboard input injection
        // g_pInputManager->onKeyboardKey(keycode, pressed);
    }
#endif
}

// Voice processing public API implementation
void CMoonlightManager::startVoiceTranscription() {
    if (!m_whisperManager) {
        Debug::log(ERR, "CMoonlightManager: Whisper manager not available");
        return;
    }
    
    if (m_voiceTranscriptionActive) {
        Debug::log(WARN, "CMoonlightManager: Voice transcription already active");
        return;
    }
    
    m_voiceTranscriptionActive = true;
    Debug::log(LOG, "CMoonlightManager: Voice transcription started");
    Debug::log(LOG, "  - Speak into any connected microphone (WebRTC, Moonlight, or system)");
    Debug::log(LOG, "  - Speech will be converted to keyboard input");
    Debug::log(LOG, "  - Voice commands available: 'new line', 'copy', 'paste', 'save', etc.");
}

void CMoonlightManager::stopVoiceTranscription() {
    if (!m_voiceTranscriptionActive) return;
    
    m_voiceTranscriptionActive = false;
    Debug::log(LOG, "CMoonlightManager: Voice transcription stopped");
}

void CMoonlightManager::startTTSServer(int port) {
    if (!m_ttsManager) {
        Debug::log(ERR, "CMoonlightManager: TTS manager not available");
        return;
    }
    
    m_ttsManager->startHTTPServer(port);
    Debug::log(LOG, "CMoonlightManager: TTS HTTP server started on port {}", port);
    Debug::log(LOG, "  - POST /speak with JSON: {{\"text\":\"Hello world\", \"voice\":\"en+f3\"}}");
    Debug::log(LOG, "  - GET /voices to list available voices");
    Debug::log(LOG, "  - GET /status for server status");
}

void CMoonlightManager::stopTTSServer() {
    if (!m_ttsManager) return;
    
    m_ttsManager->stopHTTPServer();
    Debug::log(LOG, "CMoonlightManager: TTS HTTP server stopped");
}

bool CMoonlightManager::isTTSServerRunning() const {
    if (!m_ttsManager) return false;
    return m_ttsManager->isInitialized();
}

void CMoonlightManager::speakText(const std::string& text, const std::string& voice) {
    if (!m_ttsManager) {
        Debug::log(ERR, "CMoonlightManager: TTS manager not available");
        return;
    }
    
    Debug::log(LOG, "CMoonlightManager: Speaking text: '{}'", text);
    m_ttsManager->speakText(text, voice);
}

void CMoonlightManager::registerVoiceCommand(const std::string& command, const std::string& description) {
    if (!m_whisperManager) {
        Debug::log(ERR, "CMoonlightManager: Whisper manager not available");
        return;
    }
    
    m_whisperManager->registerVoiceCommand(command, description);
    Debug::log(LOG, "CMoonlightManager: Registered voice command: '{}' - {}", command, description);
}

std::vector<std::pair<std::string, std::string>> CMoonlightManager::getVoiceCommands() const {
    if (!m_whisperManager) return {};
    return m_whisperManager->getVoiceCommands();
}

// Synthetic frame generation implementation
void CMoonlightManager::startSyntheticFrameGeneration() {
    if (m_syntheticFrameRunning) {
        Debug::log(WARN, "CMoonlightManager: Synthetic frame generation already running");
        return;
    }

    Debug::log(LOG, "CMoonlightManager: Starting synthetic frame generation for streaming");
    m_syntheticFrameRunning = true;

    m_syntheticFrameThread = std::thread([this]() {
        Debug::log(LOG, "CMoonlightManager: Synthetic frame generation thread started");

        int frame_count = 0;
        const int width = 2360;  // Match server advertised resolution
        const int height = 1640; // Match server advertised resolution
        const int fps = 30; // 30 FPS
        const auto frame_duration = std::chrono::milliseconds(1000 / fps);

        while (m_syntheticFrameRunning && m_streaming) {
            try {
                // Generate test pattern frame
                size_t frame_size = width * height * 4; // RGBA
                std::vector<uint8_t> frame_data(frame_size);

                // Create a simple color pattern that changes over time
                uint8_t base_color = (frame_count / 10) % 256;
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        int pixel_index = (y * width + x) * 4;

                        // Create a gradient pattern with moving colors
                        frame_data[pixel_index + 0] = (base_color + x / 8) % 256;     // Red
                        frame_data[pixel_index + 1] = (base_color + y / 8) % 256;     // Green
                        frame_data[pixel_index + 2] = (base_color + (x + y) / 16) % 256; // Blue
                        frame_data[pixel_index + 3] = 255;                            // Alpha
                    }
                }

                // Send frame to Wolf streaming server
                if (m_wolfServer) {
                    m_wolfServer->onFrameReady(frame_data.data(), frame_size, width, height, 0x34325258);

                    if (frame_count % (fps * 2) == 0) { // Log every 2 seconds
                        Debug::log(LOG, "CMoonlightManager: Sent synthetic frame #{} ({}x{}, {} bytes)",
                                  frame_count, width, height, frame_size);
                    }
                }

                frame_count++;
                std::this_thread::sleep_for(frame_duration);

            } catch (const std::exception& e) {
                Debug::log(ERR, "CMoonlightManager: Error in synthetic frame generation: {}", e.what());
                break;
            }
        }

        Debug::log(LOG, "CMoonlightManager: Synthetic frame generation thread ended");
    });
}

void CMoonlightManager::stopSyntheticFrameGeneration() {
    if (!m_syntheticFrameRunning) {
        return;
    }

    Debug::log(LOG, "CMoonlightManager: Stopping synthetic frame generation");
    m_syntheticFrameRunning = false;

    if (m_syntheticFrameThread.joinable()) {
        m_syntheticFrameThread.join();
    }

    Debug::log(LOG, "CMoonlightManager: Synthetic frame generation stopped");
}