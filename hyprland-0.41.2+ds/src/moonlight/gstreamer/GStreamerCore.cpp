#include "GStreamerCore.hpp"

namespace moonlight::gstreamer {

bool GStreamerCore::initialized_ = false;

void GStreamerCore::init() {
    if (initialized_) {
        return;
    }

    // Step 6: Initialize GStreamer for streaming infrastructure
    gst_init(nullptr, nullptr);
    initialized_ = true;

    Debug::log(LOG, "[moonlight] GStreamer initialized - version: {}", getVersion());
}

std::string GStreamerCore::getVersion() {
    guint major, minor, micro, nano;
    gst_version(&major, &minor, &micro, &nano);
    return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(micro) + "-" + std::to_string(nano);
}

void GStreamerCore::cleanup() {
    if (initialized_) {
        gst_deinit();
        initialized_ = false;
        Debug::log(LOG, "[moonlight] GStreamer cleanup completed");
    }
}

bool GStreamerCore::runPipeline(const std::string& pipeline_desc,
                                const std::function<void(gst_element_ptr, gst_main_loop_ptr)>& on_pipeline_ready) {
    // Step 6: Stub implementation for pipeline management
    Debug::log(LOG, "[moonlight] Pipeline requested: {}", pipeline_desc);
    Debug::log(LOG, "[moonlight] Pipeline execution stub - not implemented in Step 6");
    return true;
}

void GStreamerCore::pipelineErrorHandler(GstBus* bus, GstMessage* message, gpointer data) {
    auto loop = (GMainLoop*)data;
    GError* err;
    gchar* debug;
    gst_message_parse_error(message, &err, &debug);
    Debug::log(ERR, "[moonlight] GStreamer pipeline error: {}", err->message);
    g_error_free(err);
    g_free(debug);
    g_main_loop_quit(loop);
}

void GStreamerCore::pipelineEosHandler(GstBus* bus, GstMessage* message, gpointer data) {
    auto loop = (GMainLoop*)data;
    Debug::log(LOG, "[moonlight] GStreamer pipeline reached End Of Stream");
    g_main_loop_quit(loop);
}

void GStreamerCore::sendMessage(GstElement* recipient, GstStructure* message) {
    auto gst_ev = gst_event_new_custom(GST_EVENT_CUSTOM_UPSTREAM, message);
    gst_element_send_event(recipient, gst_ev);
}

// StreamingPipeline implementation

StreamingPipeline::StreamingPipeline() : running_(false) {
    Debug::log(LOG, "[moonlight] StreamingPipeline created");
}

StreamingPipeline::~StreamingPipeline() {
    if (running_) {
        stop();
    }
    Debug::log(LOG, "[moonlight] StreamingPipeline destroyed");
}

bool StreamingPipeline::startVideoProducer(const std::string& display_mode) {
    // Step 6: Stub implementation for video producer
    Debug::log(LOG, "[moonlight] Video producer requested - display mode: {}", display_mode);
    Debug::log(LOG, "[moonlight] Video producer stub - not implemented in Step 6");
    return true;
}

bool StreamingPipeline::startVideoStreaming(const std::string& client_ip, unsigned short port) {
    // Step 6: Stub implementation for video streaming
    Debug::log(LOG, "[moonlight] Video streaming requested - client: {}:{}", client_ip, port);
    Debug::log(LOG, "[moonlight] Video streaming stub - not implemented in Step 6");
    return true;
}

bool StreamingPipeline::startAudioProducer(const std::string& sink_name) {
    // Step 6: Stub implementation for audio producer
    Debug::log(LOG, "[moonlight] Audio producer requested - sink: {}", sink_name);
    Debug::log(LOG, "[moonlight] Audio producer stub - not implemented in Step 6");
    return true;
}

bool StreamingPipeline::startAudioStreaming(const std::string& client_ip, unsigned short port) {
    // Step 6: Stub implementation for audio streaming
    Debug::log(LOG, "[moonlight] Audio streaming requested - client: {}:{}", client_ip, port);
    Debug::log(LOG, "[moonlight] Audio streaming stub - not implemented in Step 6");
    return true;
}

void StreamingPipeline::stop() {
    if (running_) {
        running_ = false;
        Debug::log(LOG, "[moonlight] StreamingPipeline stopped");
    }
}

bool StreamingPipeline::isRunning() const {
    return running_;
}

} // namespace moonlight::gstreamer