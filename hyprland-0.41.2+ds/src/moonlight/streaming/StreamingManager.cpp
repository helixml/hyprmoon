#include "StreamingManager.hpp"
#include "../../debug/Log.hpp"

namespace moonlight::streaming {

int StreamingManager::frame_count_ = 0;

StreamingManager::StreamingManager()
    : initialized_(false), video_streaming_(false), audio_streaming_(false) {
    Debug::log(LOG, "[moonlight] StreamingManager created");
}

StreamingManager::~StreamingManager() {
    cleanup();
    Debug::log(LOG, "[moonlight] StreamingManager destroyed");
}

bool StreamingManager::initialize() {
    if (initialized_) {
        return true;
    }

    // Step 6: Initialize streaming infrastructure
    gstreamer::GStreamerCore::init();

    pipeline_ = std::make_unique<gstreamer::StreamingPipeline>();

    initialized_ = true;
    Debug::log(LOG, "[moonlight] StreamingManager initialized with GStreamer support");
    return true;
}

void StreamingManager::cleanup() {
    if (!initialized_) {
        return;
    }

    stopVideoStream();
    stopAudioStream();

    pipeline_.reset();
    gstreamer::GStreamerCore::cleanup();

    initialized_ = false;
    Debug::log(LOG, "[moonlight] StreamingManager cleanup completed");
}

bool StreamingManager::startVideoStream(const std::string& client_ip, unsigned short port) {
    if (!initialized_) {
        Debug::log(ERR, "[moonlight] Cannot start video stream - not initialized");
        return false;
    }

    if (video_streaming_) {
        Debug::log(WARN, "[moonlight] Video stream already running");
        return true;
    }

    // Step 6: Start video streaming pipeline
    Debug::log(LOG, "[moonlight] Starting video stream to {}:{}", client_ip, port);

    if (pipeline_->startVideoProducer("1920x1080") &&
        pipeline_->startVideoStreaming(client_ip, port)) {
        video_streaming_ = true;
        Debug::log(LOG, "[moonlight] Video stream started successfully");
        return true;
    }

    Debug::log(ERR, "[moonlight] Failed to start video stream");
    return false;
}

bool StreamingManager::stopVideoStream() {
    if (!video_streaming_) {
        return true;
    }

    video_streaming_ = false;
    Debug::log(LOG, "[moonlight] Video stream stopped");
    return true;
}

bool StreamingManager::startAudioStream(const std::string& client_ip, unsigned short port) {
    if (!initialized_) {
        Debug::log(ERR, "[moonlight] Cannot start audio stream - not initialized");
        return false;
    }

    if (audio_streaming_) {
        Debug::log(WARN, "[moonlight] Audio stream already running");
        return true;
    }

    // Step 6: Start audio streaming pipeline
    Debug::log(LOG, "[moonlight] Starting audio stream to {}:{}", client_ip, port);

    if (pipeline_->startAudioProducer("moonlight-sink") &&
        pipeline_->startAudioStreaming(client_ip, port)) {
        audio_streaming_ = true;
        Debug::log(LOG, "[moonlight] Audio stream started successfully");
        return true;
    }

    Debug::log(ERR, "[moonlight] Failed to start audio stream");
    return false;
}

bool StreamingManager::stopAudioStream() {
    if (!audio_streaming_) {
        return true;
    }

    audio_streaming_ = false;
    Debug::log(LOG, "[moonlight] Audio stream stopped");
    return true;
}

bool StreamingManager::isVideoStreaming() const {
    return video_streaming_;
}

bool StreamingManager::isAudioStreaming() const {
    return audio_streaming_;
}

void StreamingManager::processFrame(CMonitor* monitor, wlr_buffer* buffer) {
    if (!initialized_ || !video_streaming_) {
        return;
    }

    // Step 6: Process frame for streaming
    handleVideoFrame(monitor, buffer);
}

void StreamingManager::handleVideoFrame(CMonitor* monitor, wlr_buffer* buffer) {
    frame_count_++;

    // Log every 120th frame (2 seconds at 60fps) to avoid spam
    if (frame_count_ % 120 == 0) {
        Debug::log(LOG, "[moonlight] Processing video frame #{} from monitor: {} (streaming: {})",
                   frame_count_, monitor ? monitor->szName : "null", video_streaming_);
    }

    // Step 6: Frame processing is stubbed - will be implemented in later steps
    // This is where we would feed the frame into the GStreamer pipeline
}

} // namespace moonlight::streaming