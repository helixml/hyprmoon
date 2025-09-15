#pragma once

#include <memory>
#include <string>
#include "../gstreamer/GStreamerCore.hpp"

namespace moonlight::streaming {

/**
 * Step 6: Streaming infrastructure manager
 * Coordinates video/audio streaming pipelines and network handling
 */
class StreamingManager {
public:
    StreamingManager();
    ~StreamingManager();

    // Lifecycle
    bool initialize();
    void cleanup();

    // Video streaming control
    bool startVideoStream(const std::string& client_ip, unsigned short port);
    bool stopVideoStream();

    // Audio streaming control
    bool startAudioStream(const std::string& client_ip, unsigned short port);
    bool stopAudioStream();

    // Status
    bool isVideoStreaming() const;
    bool isAudioStreaming() const;

    // Frame input from renderer (called by MoonlightManager)
    void processFrame(class CMonitor* monitor, struct wlr_buffer* buffer);

private:
    std::unique_ptr<gstreamer::StreamingPipeline> pipeline_;
    bool initialized_;
    bool video_streaming_;
    bool audio_streaming_;

    // Frame processing
    void handleVideoFrame(class CMonitor* monitor, struct wlr_buffer* buffer);
    static int frame_count_;
};

} // namespace moonlight::streaming