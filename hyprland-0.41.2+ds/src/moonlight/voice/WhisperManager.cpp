#define GST_USE_UNSTABLE_API

#include "WhisperManager.hpp"

// Conditional includes
#ifdef HYPRLAND_INTEGRATION
#include "../../debug/Log.hpp"
// Use Hyprland's Debug namespace directly - no using statements to avoid conflicts
#else
#include <iostream>
namespace Debug {
    enum LogLevel { LOG, WARN, ERR };
    template<typename... Args>
    void log(LogLevel level, const std::string& format, Args&&... args) {
        const char* prefix = (level == ERR) ? "[ERROR] " : (level == WARN) ? "[WARN] " : "[INFO] ";
        std::cout << prefix << format << std::endl;
    }
}
// using statements removed to avoid conflicts with Hyprland's Debug namespace
#endif

// Whisper.cpp integration
#if defined(HAVE_WHISPER) && HAVE_WHISPER
#include <whisper.h>
#else
// Stub structures for compilation without whisper.cpp
struct whisper_context {};
struct whisper_state {};
struct whisper_full_params {};
#endif

// System includes
#include <cmath>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <jsoncpp/json/json.h>

// HTTP server includes
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// ===============================
// CWhisperManager Implementation
// ===============================

CWhisperManager::CWhisperManager() {
    Debug::log(Debug::LOG, "WhisperManager: Creating voice transcription manager");
    
    // Initialize default voice commands
    initializeDefaultCommands();
}

CWhisperManager::~CWhisperManager() {
    shutdown();
}

bool CWhisperManager::initialize(const std::string& model_path) {
    if (m_initialized.load()) {
        Debug::log(Debug::WARN, "WhisperManager: Already initialized");
        return true;
    }
    
    Debug::log(Debug::LOG, "WhisperManager: Initializing Whisper transcription");
    
    // Use provided model path or default
    std::string model_file = model_path.empty() ? m_config.model_path : model_path;
    
    if (!loadWhisperModel(model_file)) {
        Debug::log(Debug::ERR, "WhisperManager: Failed to load Whisper model from {}", model_file);
        return false;
    }
    
    // Start processing thread
    m_should_shutdown.store(false);
    m_processing_thread = std::thread(&CWhisperManager::processingThreadMain, this);
    
    m_initialized.store(true);
    Debug::log(Debug::LOG, "WhisperManager: Successfully initialized");
    Debug::log(Debug::LOG, "  - Model: {}", model_file);
    Debug::log(Debug::LOG, "  - Language: {}", m_config.language);
    Debug::log(Debug::LOG, "  - VAD enabled: {}", m_config.enable_vad ? "✓" : "✗");
    Debug::log(Debug::LOG, "  - Keyboard injection: {}", m_config.enable_keyboard_injection ? "✓" : "✗");
    
    return true;
}

void CWhisperManager::shutdown() {
    if (!m_initialized.load()) return;
    
    Debug::log(Debug::LOG, "WhisperManager: Shutting down");
    
    m_should_shutdown.store(true);
    
    if (m_processing_thread.joinable()) {
        m_processing_thread.join();
    }
    
    cleanupWhisper();
    m_initialized.store(false);
}

bool CWhisperManager::loadWhisperModel(const std::string& model_path) {
#if defined(HAVE_WHISPER) && HAVE_WHISPER
    // Check if model file exists
    std::ifstream file(model_path);
    if (!file.good()) {
        Debug::log(Debug::ERR, "WhisperManager: Model file not found: {}", model_path);
        return false;
    }
    
    // Initialize whisper context
    struct whisper_context_params ctx_params = whisper_context_default_params();
    ctx_params.use_gpu = m_config.use_gpu;
    
    m_whisper_ctx = whisper_init_from_file_with_params(model_path.c_str(), ctx_params);
    if (!m_whisper_ctx) {
        Debug::log(Debug::ERR, "WhisperManager: Failed to initialize whisper context");
        return false;
    }
    
    // Create whisper state
    m_whisper_state = whisper_init_state(m_whisper_ctx);
    if (!m_whisper_state) {
        Debug::log(Debug::ERR, "WhisperManager: Failed to create whisper state");
        whisper_free(m_whisper_ctx);
        m_whisper_ctx = nullptr;
        return false;
    }
    
    Debug::log(Debug::LOG, "WhisperManager: Loaded model with {} languages", 
              whisper_lang_max_id());
    return true;
#else
    Debug::log(Debug::WARN, "WhisperManager: Compiled without whisper.cpp support - using stub");
    return true;  // Return true for compilation testing
#endif
}

void CWhisperManager::cleanupWhisper() {
#if defined(HAVE_WHISPER) && HAVE_WHISPER
    if (m_whisper_state) {
        whisper_free_state(m_whisper_state);
        m_whisper_state = nullptr;
    }
    
    if (m_whisper_ctx) {
        whisper_free(m_whisper_ctx);
        m_whisper_ctx = nullptr;
    }
#endif
}

void CWhisperManager::processAudioChunk(const float* audio_data, size_t sample_count, int sample_rate) {
    if (!m_initialized.load() || !audio_data || sample_count == 0) {
        return;
    }
    
    AudioBuffer buffer;
    buffer.samples.assign(audio_data, audio_data + sample_count);
    buffer.timestamp = std::chrono::steady_clock::now();
    buffer.sample_rate = sample_rate;
    buffer.energy = calculateAudioEnergy(audio_data, sample_count);
    buffer.is_speech = detectVoiceActivity(audio_data, sample_count);
    
    std::lock_guard<std::mutex> lock(m_audio_mutex);
    m_audio_queue.push(std::move(buffer));
}

void CWhisperManager::processAudioChunk(const int16_t* audio_data, size_t sample_count, int sample_rate) {
    if (!audio_data || sample_count == 0) return;
    
    // Convert int16 to float
    std::vector<float> float_samples(sample_count);
    convertToFloat(audio_data, float_samples.data(), sample_count);
    
    processAudioChunk(float_samples.data(), sample_count, sample_rate);
}

float CWhisperManager::calculateAudioEnergy(const float* samples, size_t count) {
    if (!samples || count == 0) return 0.0f;
    
    float energy = 0.0f;
    for (size_t i = 0; i < count; ++i) {
        energy += samples[i] * samples[i];
    }
    return energy / count;
}

bool CWhisperManager::detectVoiceActivity(const float* samples, size_t count) {
    if (!m_config.enable_vad) return true;
    
    float energy = calculateAudioEnergy(samples, count);
    return energy > m_config.vad_threshold;
}

void CWhisperManager::processingThreadMain() {
    Debug::log(Debug::LOG, "WhisperManager: Processing thread started");
    
    while (!m_should_shutdown.load()) {
        AudioBuffer buffer;
        bool has_buffer = false;
        
        {
            std::lock_guard<std::mutex> lock(m_audio_mutex);
            if (!m_audio_queue.empty()) {
                buffer = std::move(m_audio_queue.front());
                m_audio_queue.pop();
                has_buffer = true;
            }
        }
        
        if (has_buffer) {
            processAudioBuffer(buffer);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
    Debug::log(Debug::LOG, "WhisperManager: Processing thread stopped");
}

void CWhisperManager::processAudioBuffer(const AudioBuffer& buffer) {
    auto now = std::chrono::steady_clock::now();
    
    // Voice activity detection
    if (buffer.is_speech) {
        if (!m_in_speech) {
            m_in_speech = true;
            m_speech_start_time = now;
            if (onSpeechActivity) {
                onSpeechActivity(true);
            }
        }
        m_last_speech_time = now;
        
        // Accumulate audio samples
        m_audio_buffer.insert(m_audio_buffer.end(), buffer.samples.begin(), buffer.samples.end());
    } else {
        // Check if we should process accumulated speech
        if (m_in_speech) {
            auto silence_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                now - m_last_speech_time).count();
            
            if (silence_duration >= m_config.silence_duration_ms) {
                // Process the accumulated speech
                if (!m_audio_buffer.empty()) {
                    auto speech_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                        m_last_speech_time - m_speech_start_time).count();
                    
                    if (speech_duration >= m_config.min_speech_duration_ms) {
                        m_processing.store(true);
                        std::string transcription = transcribeAudio(m_audio_buffer, buffer.sample_rate);
                        
                        if (!transcription.empty()) {
                            processTranscription(transcription, 1.0f);  // TODO: Get actual confidence
                        }
                        m_processing.store(false);
                    }
                }
                
                m_audio_buffer.clear();
                m_in_speech = false;
                
                if (onSpeechActivity) {
                    onSpeechActivity(false);
                }
            }
        }
    }
    
    // Prevent buffer from growing too large
    auto buffer_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - m_speech_start_time).count();
    
    if (buffer_duration > m_config.max_speech_duration_ms) {
        if (!m_audio_buffer.empty()) {
            m_processing.store(true);
            std::string transcription = transcribeAudio(m_audio_buffer, buffer.sample_rate);
            
            if (!transcription.empty()) {
                processTranscription(transcription, 1.0f);
            }
            m_processing.store(false);
        }
        
        m_audio_buffer.clear();
        m_in_speech = false;
    }
}

std::string CWhisperManager::transcribeAudio(const std::vector<float>& samples, int sample_rate) {
#if defined(HAVE_WHISPER) && HAVE_WHISPER
    if (!m_whisper_ctx || !m_whisper_state || samples.empty()) {
        return "";
    }
    
    // Resample to 16kHz if needed
    std::vector<float> resampled_samples;
    if (sample_rate != 16000) {
        resampleAudio(samples, resampled_samples, sample_rate, 16000);
    } else {
        resampled_samples = samples;
    }
    
    // Preprocess audio
    if (m_config.enable_preprocessing) {
        preprocessAudio(resampled_samples);
    }
    
    // Set up whisper parameters
    struct whisper_full_params params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
    params.language = m_config.language.c_str();
    params.translate = m_config.translate;
    params.n_threads = m_config.num_threads;
    params.print_progress = false;
    params.print_timestamps = false;
    params.print_realtime = false;
    
    // Run inference
    int result = whisper_full_with_state(m_whisper_ctx, m_whisper_state, params, 
                                        resampled_samples.data(), resampled_samples.size());
    
    if (result != 0) {
        Debug::log(Debug::ERR, "WhisperManager: Transcription failed with code {}", result);
        return "";
    }
    
    // Get transcription
    std::string transcription;
    int n_segments = whisper_full_n_segments_from_state(m_whisper_state);
    
    for (int i = 0; i < n_segments; ++i) {
        const char* text = whisper_full_get_segment_text_from_state(m_whisper_state, i);
        if (text) {
            transcription += text;
        }
    }
    
    // Post-process text
    transcription = postProcessText(transcription);
    
    Debug::log(Debug::LOG, "WhisperManager: Transcribed: '{}'", transcription);
    return transcription;
#else
    Debug::log(Debug::LOG, "WhisperManager: Stub transcription - would process {} samples", samples.size());
    return "hello world";  // Stub for testing
#endif
}

std::string CWhisperManager::postProcessText(const std::string& raw_text) {
    std::string text = raw_text;
    
    // Remove extra whitespace
    text.erase(0, text.find_first_not_of(" \t\n\r"));
    text.erase(text.find_last_not_of(" \t\n\r") + 1);
    
    if (text.empty()) return text;
    
    // Capitalize first letter
    if (m_config.enable_capitalization && !text.empty()) {
        text[0] = std::toupper(text[0]);
    }
    
    // Add period if missing
    if (m_config.enable_punctuation && !text.empty()) {
        char last_char = text.back();
        if (last_char != '.' && last_char != '!' && last_char != '?') {
            text += '.';
        }
    }
    
    return text;
}

void CWhisperManager::processTranscription(const std::string& text, float confidence) {
    if (text.empty() || confidence < m_config.confidence_threshold) {
        return;
    }
    
    Debug::log(Debug::LOG, "WhisperManager: Processing transcription: '{}' (confidence: {:.2f})", 
              text, confidence);
    
    // Call transcription callback
    if (onTranscriptionReady) {
        onTranscriptionReady(text, confidence);
    }
    
    // Check for voice commands first
    if (m_config.enable_command_detection) {
        std::string command, params;
        if (detectVoiceCommand(text, command, params)) {
            Debug::log(Debug::LOG, "WhisperManager: Detected voice command: '{}' with params: '{}'", 
                      command, params);
            
            executeVoiceCommand(command, params);
            
            if (onVoiceCommand) {
                onVoiceCommand(command, params);
            }
            return;  // Don't type the command text
        }
    }
    
    // Convert to keyboard input
    if (m_config.enable_keyboard_injection) {
        typeText(text);
    }
}

bool CWhisperManager::detectVoiceCommand(const std::string& text, std::string& command, std::string& params) {
    std::string lower_text = text;
    std::transform(lower_text.begin(), lower_text.end(), lower_text.begin(), ::tolower);
    
    std::lock_guard<std::mutex> lock(m_commands_mutex);
    
    for (const auto& [cmd, description] : m_voice_commands) {
        std::string lower_cmd = cmd;
        std::transform(lower_cmd.begin(), lower_cmd.end(), lower_cmd.begin(), ::tolower);
        
        if (lower_text.find(lower_cmd) != std::string::npos) {
            command = cmd;
            // Extract parameters (text after the command)
            size_t cmd_pos = lower_text.find(lower_cmd);
            if (cmd_pos + lower_cmd.length() < lower_text.length()) {
                params = text.substr(cmd_pos + lower_cmd.length());
                // Trim whitespace
                params.erase(0, params.find_first_not_of(" \t"));
                params.erase(params.find_last_not_of(" \t") + 1);
            }
            return true;
        }
    }
    
    return false;
}

void CWhisperManager::executeVoiceCommand(const std::string& command, const std::string& params) {
    std::string lower_cmd = command;
    std::transform(lower_cmd.begin(), lower_cmd.end(), lower_cmd.begin(), ::tolower);
    
    // Built-in commands
    if (lower_cmd == "new line" || lower_cmd == "enter") {
        injectKeystroke(10, true);   // Enter key
        injectKeystroke(10, false);
    } else if (lower_cmd == "backspace") {
        injectKeystroke(8, true);    // Backspace
        injectKeystroke(8, false);
    } else if (lower_cmd == "delete") {
        injectKeystroke(127, true);  // Delete
        injectKeystroke(127, false);
    } else if (lower_cmd == "tab") {
        injectKeystroke(9, true);    // Tab
        injectKeystroke(9, false);
    } else if (lower_cmd == "escape") {
        injectKeystroke(27, true);   // Escape
        injectKeystroke(27, false);
    } else if (lower_cmd == "space") {
        injectKeystroke(32, true);   // Space
        injectKeystroke(32, false);
    } else if (lower_cmd == "clear" || lower_cmd == "select all") {
        // Ctrl+A
        injectKeystroke(65, true, 4);   // A with Ctrl
        injectKeystroke(65, false, 4);
    } else if (lower_cmd == "copy") {
        // Ctrl+C
        injectKeystroke(67, true, 4);   // C with Ctrl
        injectKeystroke(67, false, 4);
    } else if (lower_cmd == "paste") {
        // Ctrl+V
        injectKeystroke(86, true, 4);   // V with Ctrl
        injectKeystroke(86, false, 4);
    } else if (lower_cmd == "undo") {
        // Ctrl+Z
        injectKeystroke(90, true, 4);   // Z with Ctrl
        injectKeystroke(90, false, 4);
    } else if (lower_cmd == "save") {
        // Ctrl+S
        injectKeystroke(83, true, 4);   // S with Ctrl
        injectKeystroke(83, false, 4);
    }
    
    Debug::log(Debug::LOG, "WhisperManager: Executed voice command: {}", command);
}

void CWhisperManager::typeText(const std::string& text) {
    Debug::log(Debug::LOG, "WhisperManager: Typing text: '{}'", text);
    
    for (char c : text) {
        bool needs_shift = false;
        int keycode = charToKeycode(c, needs_shift);
        
        if (keycode > 0) {
            uint32_t modifiers = needs_shift ? 2 : 0;  // Shift modifier
            
            injectKeystroke(keycode, true, modifiers);
            std::this_thread::sleep_for(std::chrono::milliseconds(m_config.typing_delay_ms));
            injectKeystroke(keycode, false, modifiers);
        }
    }
}

void CWhisperManager::injectKeystroke(int keycode, bool pressed, uint32_t modifiers) {
    if (onKeyboardEvent) {
        onKeyboardEvent(keycode, pressed, modifiers);
    }
    
    Debug::log(Debug::LOG, "WhisperManager: Injected keystroke: key={}, pressed={}, mods={}", 
              keycode, pressed, modifiers);
}

int CWhisperManager::charToKeycode(char c, bool& needs_shift) {
    needs_shift = false;
    
    // Letters
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 65;  // Convert to uppercase ASCII
    } else if (c >= 'A' && c <= 'Z') {
        needs_shift = true;
        return c;
    }
    
    // Numbers
    if (c >= '0' && c <= '9') {
        return c;
    }
    
    // Special characters
    switch (c) {
        case ' ': return 32;   // Space
        case '.': return 46;   // Period
        case ',': return 44;   // Comma
        case '!': needs_shift = true; return 49;  // 1 + Shift
        case '?': needs_shift = true; return 47;  // / + Shift
        case ':': needs_shift = true; return 59;  // ; + Shift
        case ';': return 59;
        case '\'': return 39;  // Apostrophe
        case '"': needs_shift = true; return 39;  // ' + Shift
        case '-': return 45;   // Hyphen
        case '_': needs_shift = true; return 45;  // - + Shift
        case '(': needs_shift = true; return 57;  // 9 + Shift
        case ')': needs_shift = true; return 48;  // 0 + Shift
        default: return 0;     // Unsupported character
    }
}

void CWhisperManager::initializeDefaultCommands() {
    registerVoiceCommand("new line", "Insert a new line");
    registerVoiceCommand("enter", "Press enter key");
    registerVoiceCommand("backspace", "Delete previous character");
    registerVoiceCommand("delete", "Delete next character");
    registerVoiceCommand("tab", "Insert tab character");
    registerVoiceCommand("escape", "Press escape key");
    registerVoiceCommand("space", "Insert space character");
    registerVoiceCommand("clear", "Select all text");
    registerVoiceCommand("select all", "Select all text");
    registerVoiceCommand("copy", "Copy selected text");
    registerVoiceCommand("paste", "Paste clipboard content");
    registerVoiceCommand("undo", "Undo last action");
    registerVoiceCommand("save", "Save current document");
}

void CWhisperManager::registerVoiceCommand(const std::string& command, const std::string& description) {
    std::lock_guard<std::mutex> lock(m_commands_mutex);
    m_voice_commands.emplace_back(command, description);
}

void CWhisperManager::clearVoiceCommands() {
    std::lock_guard<std::mutex> lock(m_commands_mutex);
    m_voice_commands.clear();
}

std::vector<std::pair<std::string, std::string>> CWhisperManager::getVoiceCommands() const {
    std::lock_guard<std::mutex> lock(m_commands_mutex);
    return m_voice_commands;
}

size_t CWhisperManager::getQueueSize() const {
    std::lock_guard<std::mutex> lock(m_audio_mutex);
    return m_audio_queue.size();
}

void CWhisperManager::convertToFloat(const int16_t* input, float* output, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        output[i] = static_cast<float>(input[i]) / 32768.0f;
    }
}

void CWhisperManager::preprocessAudio(std::vector<float>& samples) {
    if (samples.empty()) return;
    
    // Normalize audio
    normalizeAudio(samples);
    
    // Apply high-pass filter to remove low-frequency noise
    applyHighPassFilter(samples, 80.0f, 16000);
}

void CWhisperManager::normalizeAudio(std::vector<float>& samples) {
    if (samples.empty()) return;
    
    // Find peak amplitude
    float peak = 0.0f;
    for (float sample : samples) {
        peak = std::max(peak, std::abs(sample));
    }
    
    // Normalize to 0.9 to prevent clipping
    if (peak > 0.0f) {
        float scale = 0.9f / peak;
        for (float& sample : samples) {
            sample *= scale;
        }
    }
}

void CWhisperManager::applyHighPassFilter(std::vector<float>& samples, float cutoff_freq, int sample_rate) {
    if (samples.empty()) return;
    
    // Simple high-pass filter implementation
    float rc = 1.0f / (2.0f * M_PI * cutoff_freq);
    float dt = 1.0f / sample_rate;
    float alpha = rc / (rc + dt);
    
    float prev_input = 0.0f;
    float prev_output = 0.0f;
    
    for (float& sample : samples) {
        float output = alpha * (prev_output + sample - prev_input);
        prev_input = sample;
        prev_output = output;
        sample = output;
    }
}

void CWhisperManager::resampleAudio(const std::vector<float>& input, std::vector<float>& output, 
                                   int input_rate, int output_rate) {
    if (input_rate == output_rate) {
        output = input;
        return;
    }
    
    // Simple linear interpolation resampling
    float ratio = static_cast<float>(input_rate) / output_rate;
    size_t output_size = static_cast<size_t>(input.size() / ratio);
    output.resize(output_size);
    
    for (size_t i = 0; i < output_size; ++i) {
        float src_index = i * ratio;
        size_t src_i = static_cast<size_t>(src_index);
        float frac = src_index - src_i;
        
        if (src_i + 1 < input.size()) {
            output[i] = input[src_i] * (1.0f - frac) + input[src_i + 1] * frac;
        } else {
            output[i] = input[src_i];
        }
    }
}

// ===============================
// CTTSManager Implementation  
// ===============================

CTTSManager::CTTSManager() {
    Debug::log(Debug::LOG, "TTSManager: Creating text-to-speech manager");
}

CTTSManager::~CTTSManager() {
    shutdown();
}

bool CTTSManager::initialize() {
    if (m_initialized.load()) {
        Debug::log(Debug::WARN, "TTSManager: Already initialized");
        return true;
    }
    
    Debug::log(Debug::LOG, "TTSManager: Initializing TTS system");
    
    // Initialize PulseAudio
    if (!initializePulseAudio()) {
        Debug::log(Debug::ERR, "TTSManager: Failed to initialize PulseAudio");
        return false;
    }
    
    // Start TTS processing thread
    m_should_shutdown.store(false);
    m_tts_thread = std::thread(&CTTSManager::ttsThreadMain, this);
    
    m_initialized.store(true);
    Debug::log(Debug::LOG, "TTSManager: Successfully initialized");
    Debug::log(Debug::LOG, "  - TTS Engine: {}", m_config.tts_engine);
    Debug::log(Debug::LOG, "  - Default Voice: {}", m_config.default_voice);
    Debug::log(Debug::LOG, "  - Audio Device: {}", m_config.audio_device);
    
    return true;
}

void CTTSManager::shutdown() {
    if (!m_initialized.load()) return;
    
    Debug::log(Debug::LOG, "TTSManager: Shutting down");
    
    stopHTTPServer();
    
    m_should_shutdown.store(true);
    
    if (m_tts_thread.joinable()) {
        m_tts_thread.join();
    }
    
    // Cleanup PulseAudio
    if (m_pulse_simple) {
        pa_simple_free((pa_simple*)m_pulse_simple);
        m_pulse_simple = nullptr;
    }
    
    m_initialized.store(false);
}

void CTTSManager::speakText(const std::string& text, const std::string& voice) {
    if (!m_initialized.load() || text.empty()) return;
    
    std::lock_guard<std::mutex> lock(m_speech_mutex);
    m_speech_queue.emplace(text, voice.empty() ? m_config.default_voice : voice);
}

void CTTSManager::speakTextAsync(const std::string& text, const std::string& voice) {
    speakText(text, voice);  // Same implementation for now
}

void CTTSManager::startHTTPServer(int port) {
    if (m_http_running.load()) {
        Debug::log(Debug::WARN, "TTSManager: HTTP server already running");
        return;
    }
    
    m_config.http_port = port;
    m_http_running.store(true);
    m_http_thread = std::thread(&CTTSManager::httpServerMain, this);
    
    Debug::log(Debug::LOG, "TTSManager: HTTP server starting on port {}", port);
}

void CTTSManager::stopHTTPServer() {
    if (!m_http_running.load()) return;
    
    m_http_running.store(false);
    
    if (m_http_thread.joinable()) {
        m_http_thread.join();
    }
    
    Debug::log(Debug::LOG, "TTSManager: HTTP server stopped");
}

bool CTTSManager::initializePulseAudio() {
    pa_sample_spec sample_spec;
    sample_spec.format = PA_SAMPLE_FLOAT32LE;
    sample_spec.channels = m_config.channels;
    sample_spec.rate = m_config.sample_rate;
    
    pa_buffer_attr buffer_attr;
    buffer_attr.maxlength = m_config.buffer_size * 4;  // 4 bytes per float32 sample
    buffer_attr.tlength = buffer_attr.maxlength;
    buffer_attr.prebuf = buffer_attr.maxlength / 2;
    buffer_attr.minreq = buffer_attr.maxlength / 4;
    buffer_attr.fragsize = (uint32_t)-1;  // Not used for playback
    
    int error;
    m_pulse_simple = pa_simple_new(
        nullptr,                    // Default server
        "HyprMoon TTS",            // Application name
        PA_STREAM_PLAYBACK,        // Stream direction
        m_config.audio_device.c_str(),  // Device
        "TTS Output",              // Stream description
        &sample_spec,              // Sample format
        nullptr,                   // Channel map (default)
        &buffer_attr,              // Buffer attributes
        &error                     // Error code
    );
    
    if (!m_pulse_simple) {
        Debug::log(Debug::ERR, "TTSManager: Failed to create PulseAudio stream: {}", pa_strerror(error));
        return false;
    }
    
    return true;
}

void CTTSManager::ttsThreadMain() {
    Debug::log(Debug::LOG, "TTSManager: TTS thread started");
    
    while (!m_should_shutdown.load()) {
        processSpeechQueue();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    Debug::log(Debug::LOG, "TTSManager: TTS thread stopped");
}

void CTTSManager::processSpeechQueue() {
    std::pair<std::string, std::string> speech_item;
    bool has_item = false;
    
    {
        std::lock_guard<std::mutex> lock(m_speech_mutex);
        if (!m_speech_queue.empty()) {
            speech_item = m_speech_queue.front();
            m_speech_queue.pop();
            has_item = true;
        }
    }
    
    if (!has_item) return;
    
    const std::string& text = speech_item.first;
    const std::string& voice = speech_item.second;
    
    Debug::log(Debug::LOG, "TTSManager: Synthesizing: '{}' with voice '{}'", text, voice);
    
    m_speaking.store(true);
    
    if (onSpeechStarted) {
        onSpeechStarted(text);
    }
    
    // Synthesize speech
    std::vector<float> audio_data;
    if (m_config.tts_engine == "espeak-ng") {
        audio_data = synthesizeWithEspeak(text, voice);
    } else if (m_config.tts_engine == "festival") {
        audio_data = synthesizeWithFestival(text, voice);
    } else if (m_config.tts_engine == "piper") {
        audio_data = synthesizeWithPiper(text, voice);
    }
    
    if (!audio_data.empty()) {
        // Apply prosody modifications
        if (m_config.enable_prosody) {
            applyProsody(audio_data);
        }
        
        // Call audio generated callback
        if (onAudioGenerated) {
            onAudioGenerated(audio_data, m_config.sample_rate);
        }
        
        // Play audio
        playAudio(audio_data);
    }
    
    m_speaking.store(false);
    
    if (onSpeechFinished) {
        onSpeechFinished(text);
    }
}

std::vector<float> CTTSManager::synthesizeWithEspeak(const std::string& text, const std::string& voice) {
    // Create temporary file for audio output
    std::string temp_file = "/tmp/hyprmoon_tts_" + std::to_string(getpid()) + ".wav";
    
    // Build espeak command
    std::ostringstream cmd;
    cmd << "espeak-ng -v " << voice 
        << " -s " << (int)(m_config.speech_rate * 175)
        << " -p " << (int)(m_config.pitch * 50)
        << " -a " << (int)(m_config.volume * 100)
        << " -w " << temp_file
        << " \"" << text << "\"";
    
    int result = system(cmd.str().c_str());
    if (result != 0) {
        Debug::log(Debug::ERR, "TTSManager: espeak-ng failed with code {}", result);
        return {};
    }
    
    // Load the generated audio file (simplified - would need proper WAV parsing)
    std::ifstream file(temp_file, std::ios::binary);
    if (!file.good()) {
        Debug::log(Debug::ERR, "TTSManager: Failed to read generated audio file");
        unlink(temp_file.c_str());
        return {};
    }
    
    // Skip WAV header (44 bytes) and read audio data
    file.seekg(44);
    std::vector<int16_t> raw_data((std::istreambuf_iterator<char>(file)), 
                                  std::istreambuf_iterator<char>());
    file.close();
    unlink(temp_file.c_str());
    
    // Convert to float
    std::vector<float> audio_data(raw_data.size() / 2);  // 16-bit samples
    for (size_t i = 0; i < audio_data.size(); ++i) {
        audio_data[i] = static_cast<float>(raw_data[i]) / 32768.0f;
    }
    
    return audio_data;
}

std::vector<float> CTTSManager::synthesizeWithFestival(const std::string& text, const std::string& voice) {
    // Placeholder - would implement Festival TTS integration
    Debug::log(Debug::LOG, "TTSManager: Festival synthesis not implemented yet");
    return {};
}

std::vector<float> CTTSManager::synthesizeWithPiper(const std::string& text, const std::string& voice) {
    // Placeholder - would implement Piper TTS integration
    Debug::log(Debug::LOG, "TTSManager: Piper synthesis not implemented yet");
    return {};
}

void CTTSManager::playAudio(const std::vector<float>& audio_data) {
    if (!m_pulse_simple || audio_data.empty()) return;
    
    int error;
    if (pa_simple_write((pa_simple*)m_pulse_simple, audio_data.data(), 
                       audio_data.size() * sizeof(float), &error) < 0) {
        Debug::log(Debug::ERR, "TTSManager: Failed to write audio data: {}", pa_strerror(error));
    }
}

void CTTSManager::applyProsody(std::vector<float>& audio_data) {
    // Apply volume scaling
    for (float& sample : audio_data) {
        sample *= m_config.volume;
    }
    
    // TODO: Implement pitch and rate modifications
}

void CTTSManager::httpServerMain() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        Debug::log(Debug::ERR, "TTSManager: Failed to create socket");
        return;
    }
    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(m_config.http_port);
    
    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        Debug::log(Debug::ERR, "TTSManager: Failed to bind socket to port {}", m_config.http_port);
        close(server_fd);
        return;
    }
    
    if (listen(server_fd, 10) < 0) {
        Debug::log(Debug::ERR, "TTSManager: Failed to listen on socket");
        close(server_fd);
        return;
    }
    
    Debug::log(Debug::LOG, "TTSManager: HTTP server listening on port {}", m_config.http_port);
    
    while (m_http_running.load()) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            if (m_http_running.load()) {
                Debug::log(Debug::ERR, "TTSManager: Failed to accept connection");
            }
            continue;
        }
        
        // Read HTTP request (simplified)
        char buffer[4096] = {0};
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
        read(client_fd, buffer, sizeof(buffer) - 1);
#pragma GCC diagnostic pop
        
        std::string request(buffer);
        std::string response;
        
        if (request.find("POST /speak") == 0) {
            size_t body_start = request.find("\r\n\r\n");
            if (body_start != std::string::npos) {
                std::string body = request.substr(body_start + 4);
                handleSpeakRequest(body, response);
            }
        } else if (request.find("GET /voices") == 0) {
            handleVoicesRequest(response);
        } else if (request.find("GET /status") == 0) {
            handleStatusRequest(response);
        } else {
            response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        }
        
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
        write(client_fd, response.c_str(), response.length());
#pragma GCC diagnostic pop
        close(client_fd);
    }
    
    close(server_fd);
    Debug::log(Debug::LOG, "TTSManager: HTTP server thread stopped");
}

void CTTSManager::handleSpeakRequest(const std::string& request_body, std::string& response) {
    try {
        Json::Value root;
        Json::CharReaderBuilder builder;
        std::string errors;
        std::istringstream stream(request_body);
        
        if (!Json::parseFromStream(builder, stream, &root, &errors)) {
            response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
            return;
        }
        
        std::string text = root.get("text", "").asString();
        std::string voice = root.get("voice", m_config.default_voice).asString();
        
        if (!text.empty()) {
            speakText(text, voice);
            response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n";
            if (m_config.enable_cors) {
                response += "Access-Control-Allow-Origin: *\r\n";
            }
            response += "Content-Length: 15\r\n\r\n{\"status\":\"ok\"}";
        } else {
            response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        }
    } catch (const std::exception& e) {
        Debug::log(Debug::ERR, "TTSManager: Error handling speak request: {}", e.what());
        response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
    }
}

void CTTSManager::handleVoicesRequest(std::string& response) {
    Json::Value voices(Json::arrayValue);
    auto available_voices = getAvailableVoices();
    
    for (const auto& voice : available_voices) {
        Json::Value voice_obj;
        voice_obj["id"] = voice.id;
        voice_obj["name"] = voice.name;
        voice_obj["language"] = voice.language;
        voice_obj["gender"] = voice.gender;
        voice_obj["description"] = voice.description;
        voices.append(voice_obj);
    }
    
    Json::StreamWriterBuilder builder;
    std::string json_response = Json::writeString(builder, voices);
    
    response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n";
    if (m_config.enable_cors) {
        response += "Access-Control-Allow-Origin: *\r\n";
    }
    response += "Content-Length: " + std::to_string(json_response.length()) + "\r\n\r\n" + json_response;
}

void CTTSManager::handleStatusRequest(std::string& response) {
    Json::Value status;
    status["initialized"] = m_initialized.load();
    status["speaking"] = m_speaking.load();
    status["queue_size"] = static_cast<int>(m_speech_queue.size());
    status["tts_engine"] = m_config.tts_engine;
    status["default_voice"] = m_config.default_voice;
    
    Json::StreamWriterBuilder builder;
    std::string json_response = Json::writeString(builder, status);
    
    response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n";
    if (m_config.enable_cors) {
        response += "Access-Control-Allow-Origin: *\r\n";
    }
    response += "Content-Length: " + std::to_string(json_response.length()) + "\r\n\r\n" + json_response;
}

std::vector<CTTSManager::VoiceInfo> CTTSManager::getAvailableVoices() const {
    return detectAvailableVoices();
}

std::vector<CTTSManager::VoiceInfo> CTTSManager::detectAvailableVoices() const {
    std::vector<VoiceInfo> voices;
    
    // Add some common espeak-ng voices
    voices.push_back({"en+f3", "English Female 3", "en", "female", "Default English female voice"});
    voices.push_back({"en+m3", "English Male 3", "en", "male", "Default English male voice"});
    voices.push_back({"en-us+f1", "US English Female 1", "en-US", "female", "US English female voice"});
    voices.push_back({"en-us+m1", "US English Male 1", "en-US", "male", "US English male voice"});
    
    return voices;
}

bool CTTSManager::isSpeaking() const {
    return m_speaking.load();
}

void CTTSManager::stopSpeaking() {
    std::lock_guard<std::mutex> lock(m_speech_mutex);
    while (!m_speech_queue.empty()) {
        m_speech_queue.pop();
    }
}

void CTTSManager::setVoice(const std::string& voice_id) {
    m_config.default_voice = voice_id;
}