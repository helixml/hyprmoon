#pragma once

#include <gst/gst.h>
#include <memory>
#include <string>
#include <functional>
#include "../../debug/Log.hpp"

namespace moonlight::gstreamer {

using gst_element_ptr = std::shared_ptr<GstElement>;
using gst_main_loop_ptr = std::shared_ptr<GMainLoop>;

/**
 * Step 6: Basic GStreamer integration for Moonlight streaming
 * This provides the minimal infrastructure for video/audio streaming pipelines
 */
class GStreamerCore {
public:
    static void init();
    static std::string getVersion();
    static void cleanup();

    // Pipeline management
    static bool runPipeline(const std::string& pipeline_desc,
                           const std::function<void(gst_element_ptr, gst_main_loop_ptr)>& on_pipeline_ready);

    // Error handling
    static void pipelineErrorHandler(GstBus* bus, GstMessage* message, gpointer data);
    static void pipelineEosHandler(GstBus* bus, GstMessage* message, gpointer data);

    // Message handling
    static void sendMessage(GstElement* recipient, GstStructure* message);

private:
    static bool initialized_;
};

/**
 * Streaming pipeline management for video and audio
 */
class StreamingPipeline {
public:
    StreamingPipeline();
    ~StreamingPipeline();

    // Video streaming
    bool startVideoProducer(const std::string& display_mode);
    bool startVideoStreaming(const std::string& client_ip, unsigned short port);

    // Audio streaming
    bool startAudioProducer(const std::string& sink_name);
    bool startAudioStreaming(const std::string& client_ip, unsigned short port);

    // Control
    void stop();
    bool isRunning() const;

private:
    gst_element_ptr video_pipeline_;
    gst_element_ptr audio_pipeline_;
    gst_main_loop_ptr main_loop_;
    bool running_;
};

} // namespace moonlight::gstreamer