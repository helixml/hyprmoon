#pragma once

#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <functional>
#include <chrono>

// Forward declaration for whisper.cpp
struct whisper_context;
struct whisper_state;

/**
 * Whisper Audio Transcription Manager
 * 
 * Processes audio input from WebRTC/Moonlight clients and converts speech to text,
 * then transforms text into keyboard input events for seamless voice control.
 */
class CWhisperManager {
public:
    CWhisperManager();
    ~CWhisperManager();
    
    // Lifecycle
    bool initialize(const std::string& model_path = "");
    void shutdown();
    
    // Audio processing
    void processAudioChunk(const float* audio_data, size_t sample_count, int sample_rate = 16000);
    void processAudioChunk(const int16_t* audio_data, size_t sample_count, int sample_rate = 16000);
    
    // Configuration
    struct Config {
        std::string model_path = "models/ggml-base.en.bin";  // Whisper model path
        std::string language = "en";                         // Language code
        bool translate = false;                              // Translate to English if non-English
        
        // Voice activity detection
        bool enable_vad = true;                              // Voice activity detection
        float vad_threshold = 0.1f;                          // Energy threshold for VAD
        int min_speech_duration_ms = 500;                   // Minimum speech duration
        int max_speech_duration_ms = 10000;                 // Maximum speech duration
        int silence_duration_ms = 1000;                     // Silence before processing
        
        // Audio processing
        int audio_buffer_ms = 3000;                         // Audio buffer duration
        bool enable_preprocessing = true;                    // Audio preprocessing (denoise, etc.)
        
        // Text processing
        bool enable_punctuation = true;                     // Add punctuation
        bool enable_capitalization = true;                  // Proper capitalization
        float confidence_threshold = 0.7f;                  // Minimum confidence for output
        
        // Keyboard injection
        bool enable_keyboard_injection = true;              // Convert text to keystrokes
        bool enable_command_detection = true;               // Detect voice commands
        int typing_delay_ms = 50;                           // Delay between keystrokes
        
        // Performance
        int num_threads = 4;                                // Number of processing threads
        bool use_gpu = true;                                // Use GPU acceleration if available
    } m_config;
    
    // Callbacks
    std::function<void(const std::string& text, float confidence)> onTranscriptionReady;
    std::function<void(int keycode, bool pressed, uint32_t modifiers)> onKeyboardEvent;
    std::function<void(const std::string& command, const std::string& params)> onVoiceCommand;
    std::function<void(bool speaking)> onSpeechActivity;
    
    // Status
    bool isInitialized() const { return m_initialized.load(); }
    bool isProcessing() const { return m_processing.load(); }
    size_t getQueueSize() const;
    
    // Voice commands
    void registerVoiceCommand(const std::string& command, const std::string& description);
    void clearVoiceCommands();
    std::vector<std::pair<std::string, std::string>> getVoiceCommands() const;

private:
    // Audio buffer management
    struct AudioBuffer {
        std::vector<float> samples;
        std::chrono::steady_clock::time_point timestamp;
        int sample_rate;
        bool is_speech = false;
        float energy = 0.0f;
    };
    
    // State
    std::atomic<bool> m_initialized{false};
    std::atomic<bool> m_processing{false};
    std::atomic<bool> m_should_shutdown{false};
    
    // Whisper context
    whisper_context* m_whisper_ctx = nullptr;
    whisper_state* m_whisper_state = nullptr;
    
    // Audio processing
    std::queue<AudioBuffer> m_audio_queue;
    mutable std::mutex m_audio_mutex;
    std::thread m_processing_thread;
    
    std::vector<float> m_audio_buffer;          // Accumulating audio buffer
    std::chrono::steady_clock::time_point m_last_speech_time;
    std::chrono::steady_clock::time_point m_speech_start_time;
    bool m_in_speech = false;
    
    // Voice activity detection
    float calculateAudioEnergy(const float* samples, size_t count);
    bool detectVoiceActivity(const float* samples, size_t count);
    
    // Audio preprocessing
    void preprocessAudio(std::vector<float>& samples);
    void normalizeAudio(std::vector<float>& samples);
    void applyHighPassFilter(std::vector<float>& samples, float cutoff_freq, int sample_rate);
    
    // Processing
    void processingThreadMain();
    void processAudioBuffer(const AudioBuffer& buffer);
    std::string transcribeAudio(const std::vector<float>& samples, int sample_rate);
    
    // Text processing
    std::string postProcessText(const std::string& raw_text);
    void processTranscription(const std::string& text, float confidence);
    
    // Voice commands
    bool detectVoiceCommand(const std::string& text, std::string& command, std::string& params);
    void executeVoiceCommand(const std::string& command, const std::string& params);
    
    // Keyboard injection
    void typeText(const std::string& text);
    void injectKeystroke(int keycode, bool pressed, uint32_t modifiers = 0);
    int charToKeycode(char c, bool& needs_shift);
    
    // Voice command registry
    std::vector<std::pair<std::string, std::string>> m_voice_commands;
    mutable std::mutex m_commands_mutex;
    
    // Internal helpers
    void initializeDefaultCommands();
    bool loadWhisperModel(const std::string& model_path);
    void cleanupWhisper();
    
    // Audio format conversion
    void convertToFloat(const int16_t* input, float* output, size_t count);
    void resampleAudio(const std::vector<float>& input, std::vector<float>& output, 
                      int input_rate, int output_rate);
};

/**
 * Text-to-Speech Voice Output Manager
 * 
 * Provides HTTP API for sending text that gets converted to speech
 * and mixed into the audio output stream.
 */
class CTTSManager {
public:
    CTTSManager();
    ~CTTSManager();
    
    // Lifecycle
    bool initialize();
    void shutdown();
    
    // Speech synthesis
    void speakText(const std::string& text, const std::string& voice = "default");
    void speakTextAsync(const std::string& text, const std::string& voice = "default");
    bool isSpeaking() const;
    void stopSpeaking();
    
    // Status
    bool isInitialized() const { return m_initialized.load(); }
    
    // HTTP API
    void startHTTPServer(int port = 8080);
    void stopHTTPServer();
    
    // Configuration
    struct Config {
        std::string tts_engine = "espeak-ng";              // TTS engine (espeak-ng, festival, piper)
        std::string default_voice = "en+f3";               // Default voice
        float speech_rate = 1.0f;                          // Speech rate multiplier
        float pitch = 1.0f;                                // Pitch multiplier
        float volume = 0.8f;                               // Volume level (0.0-1.0)
        
        // Audio output
        std::string audio_device = "default";              // PulseAudio device
        int sample_rate = 22050;                           // Output sample rate
        int channels = 1;                                  // Output channels
        
        // HTTP server
        int http_port = 8080;                              // HTTP server port
        std::string bind_address = "0.0.0.0";             // Bind address
        bool enable_cors = true;                           // Enable CORS headers
        
        // Voice processing
        bool enable_ssml = true;                           // Support SSML markup
        bool enable_prosody = true;                        // Prosody control
        std::string audio_format = "wav";                 // Output audio format
        
        // Performance
        bool stream_audio = true;                          // Stream audio as it's generated
        int buffer_size = 4096;                           // Audio buffer size
    } m_config;
    
    // Available voices
    struct VoiceInfo {
        std::string id;
        std::string name;
        std::string language;
        std::string gender;
        std::string description;
    };
    
    std::vector<VoiceInfo> getAvailableVoices() const;
    void setVoice(const std::string& voice_id);
    
    // Callbacks
    std::function<void(const std::string& text)> onSpeechStarted;
    std::function<void(const std::string& text)> onSpeechFinished;
    std::function<void(const std::vector<float>& audio_data, int sample_rate)> onAudioGenerated;

private:
    // State
    std::atomic<bool> m_initialized{false};
    std::atomic<bool> m_speaking{false};
    std::atomic<bool> m_should_shutdown{false};
    
    // TTS processing
    std::queue<std::pair<std::string, std::string>> m_speech_queue;  // text, voice
    mutable std::mutex m_speech_mutex;
    std::thread m_tts_thread;
    
    // HTTP server
    std::thread m_http_thread;
    std::atomic<bool> m_http_running{false};
    
    // Audio output
    void* m_pulse_simple = nullptr;  // PulseAudio simple connection
    
    // Processing
    void ttsThreadMain();
    void httpServerMain();
    void processSpeechQueue();
    
    // TTS engines
    std::vector<float> synthesizeWithEspeak(const std::string& text, const std::string& voice);
    std::vector<float> synthesizeWithFestival(const std::string& text, const std::string& voice);
    std::vector<float> synthesizeWithPiper(const std::string& text, const std::string& voice);
    
    // Audio output
    bool initializePulseAudio();
    void playAudio(const std::vector<float>& audio_data);
    void mixAudioToOutput(const std::vector<float>& audio_data);
    
    // HTTP handlers
    void handleSpeakRequest(const std::string& request_body, std::string& response);
    void handleVoicesRequest(std::string& response);
    void handleStatusRequest(std::string& response);
    
    // Utility
    std::string processSSML(const std::string& text);
    void applyProsody(std::vector<float>& audio_data);
    std::string urlDecode(const std::string& encoded);
    std::vector<VoiceInfo> detectAvailableVoices() const;
};