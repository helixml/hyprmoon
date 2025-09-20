#pragma once

#include <memory>
#include <string>
#include <wlr/interfaces/wlr_buffer.h>
#include <wlr/types/wlr_buffer.h>
#include <wlr/types/wlr_linux_dmabuf_v1.h>
#include <drm_fourcc.h>
#include "helpers/Monitor.hpp"
#include "helpers/memory/Memory.hpp"

// Forward declarations for Wolf components
namespace wolf {
namespace core {
    class WolfMoonlightServer;
    class MoonlightConfig;
}
}

// Forward declaration for WebRTC
class CWebRTCManager;

// Forward declarations for Voice Processing
class CWhisperManager;
class CTTSManager;

class CMoonlightManager {
public:
    CMoonlightManager();
    ~CMoonlightManager();

    void init();
    void destroy();
    
    // Streaming control
    void startStreaming(CMonitor* monitor = nullptr);
    void stopStreaming();
    bool isStreaming() const { return m_streaming; }

    // Session management for Wolf integration
    std::string createWolfSession(const std::string& client_cert, const std::string& client_ip);
    void startStreamingSession(const std::string& session_id);
    void stopStreamingSession(const std::string& session_id);
    
    // WebRTC streaming control
    void startWebRTCStreaming();
    void stopWebRTCStreaming();
    bool isWebRTCStreaming() const { return m_webrtcStreaming; }
    
    // WebRTC session management
    std::string createWebRTCOffer(const std::string& clientId);
    bool setWebRTCAnswer(const std::string& clientId, const std::string& sdp);
    bool addWebRTCICECandidate(const std::string& clientId, const std::string& candidate);
    void closeWebRTCSession(const std::string& clientId);
    
    // WebRTC status
    size_t getWebRTCClientCount() const;
    std::vector<std::string> getActiveWebRTCClients() const;
    
    // Voice processing control
    void startVoiceTranscription();
    void stopVoiceTranscription();
    bool isVoiceTranscriptionActive() const { return m_voiceTranscriptionActive; }
    
    void startTTSServer(int port = 8080);
    void stopTTSServer();
    bool isTTSServerRunning() const;
    
    // Voice processing API
    void speakText(const std::string& text, const std::string& voice = "default");
    void registerVoiceCommand(const std::string& command, const std::string& description);
    std::vector<std::pair<std::string, std::string>> getVoiceCommands() const;
    
    // Frame callback from renderer
    bool onFrameReady(CMonitor* monitor, wlr_buffer* buffer); // Returns true if took buffer ownership

    // Synthetic frame generation (fallback when no real frames)
    void startSyntheticFrameGeneration();
    void stopSyntheticFrameGeneration();

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
    bool processFrame(wlr_buffer* buffer); // Returns true if took buffer ownership
    bool extractFrameData(wlr_buffer* buffer, void** frame_data, size_t* frame_size);
    bool extractDMABufInfo(wlr_buffer* buffer, int* fd, uint32_t* stride, uint64_t* modifier);
    void setupFrameSource();
    
    // WebRTC integration
    void setupWebRTCIntegration();
    void setupSharedPipeline();
    void handleWebRTCInput(int keycode, bool pressed, uint32_t modifiers);
    void handleWebRTCMouse(double x, double y, int button, bool pressed);
    void handleWebRTCAudio(const void* audio_data, size_t size, int channels, int sample_rate);
    
    // Voice processing integration
    void setupVoiceProcessing();
    void handleVoiceTranscription(const std::string& text, float confidence);
    void handleVoiceCommand(const std::string& command, const std::string& params);
    void handleVoiceKeyboardEvent(int keycode, bool pressed, uint32_t modifiers);
    void processAudioForVoice(const void* audio_data, size_t size, int channels, int sample_rate);
    void routeAudioToWhisper(const float* audio_data, size_t sample_count, int sample_rate);
    
    // State
    bool m_initialized = false;
    bool m_streaming = false;
    bool m_webrtcStreaming = false;
    bool m_voiceTranscriptionActive = false;
    CMonitor* m_streamingMonitor = nullptr;

    // Session-based frame generation tracking
    std::atomic<int> m_activeSessionCount{0};

    // Synthetic frame generation
    std::thread m_syntheticFrameThread;
    std::atomic<bool> m_syntheticFrameRunning{false};
    
    // Wolf moonlight server (using pimpl pattern to avoid header dependencies)
    std::unique_ptr<wolf::core::WolfMoonlightServer> m_wolfServer;
    
    // WebRTC manager
    std::unique_ptr<CWebRTCManager> m_webrtcManager;
    
    // Voice processing managers
    std::unique_ptr<CWhisperManager> m_whisperManager;
    std::unique_ptr<CTTSManager> m_ttsManager;
    
    // GStreamer pipeline (shared between Wolf and WebRTC)
    void* m_pipeline = nullptr; // GstElement* (avoiding gstreamer headers here)
    void* m_frameSrc = nullptr; // Custom frame source element
    void* m_teeElement = nullptr; // Tee for splitting frames to Wolf and WebRTC
    
    // Configuration
    struct Config {
        // Wolf/Moonlight settings
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
        
        // WebRTC settings
        bool webrtcEnabled = true;
        bool webrtcAudioInput = true;
        bool webrtcAudioOutput = true;
        bool webrtcKeyboardInput = true;
        bool webrtcMouseInput = true;
        std::string stunServer = "stun://stun.l.google.com:19302";
        
        // Voice processing settings
        bool voiceTranscriptionEnabled = true;
        bool ttsEnabled = true;
        std::string whisperModelPath = "models/ggml-base.en.bin";
        std::string ttsEngine = "espeak-ng";
        std::string defaultVoice = "en+f3";
        int ttsPort = 8080;
        bool voiceCommandsEnabled = true;
    } m_config;
};

// Global instance
extern UP<CMoonlightManager> g_pMoonlightManager;