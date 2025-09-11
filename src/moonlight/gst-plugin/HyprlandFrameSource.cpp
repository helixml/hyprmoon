#include "HyprlandFrameSource.hpp"
#include "../../debug/Log.hpp"
#include <gst/app/gstappsrc.h>
#include <gst/video/video.h>
#include <sys/mman.h>
#include <unistd.h>
#include <drm_fourcc.h>

GST_DEBUG_CATEGORY_STATIC(gst_hyprland_frame_src_debug);
#define GST_CAT_DEFAULT gst_hyprland_frame_src_debug

// Property IDs
enum {
    PROP_0,
    PROP_WIDTH,
    PROP_HEIGHT,
    PROP_FRAMERATE,
    PROP_FORMAT,
    PROP_MONITOR
};

// Default values
#define DEFAULT_WIDTH 1920
#define DEFAULT_HEIGHT 1080
#define DEFAULT_FRAMERATE_NUM 60
#define DEFAULT_FRAMERATE_DEN 1
#define DEFAULT_FORMAT "RGBx"

// Function prototypes
static void gst_hyprland_frame_src_class_init(GstHyprlandFrameSrcClass* klass);
static void gst_hyprland_frame_src_init(GstHyprlandFrameSrc* src);
static void gst_hyprland_frame_src_finalize(GObject* object);

static void gst_hyprland_frame_src_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec);
static void gst_hyprland_frame_src_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec);

static GstCaps* gst_hyprland_frame_src_get_caps(GstBaseSrc* src, GstCaps* filter);
static gboolean gst_hyprland_frame_src_start(GstBaseSrc* src);
static gboolean gst_hyprland_frame_src_stop(GstBaseSrc* src);
static GstFlowReturn gst_hyprland_frame_src_create(GstBaseSrc* src, guint64 offset, guint size, GstBuffer** buffer);

// Helper functions
static GstBuffer* aquamarine_buffer_to_gst_buffer(SP<Aquamarine::IBuffer> aq_buffer, guint width, guint height);
static GstVideoFormat drm_format_to_gst_format(uint32_t drm_format);

// GObject type definition
G_DEFINE_TYPE(GstHyprlandFrameSrc, gst_hyprland_frame_src, GST_TYPE_BASE_SRC);

static void gst_hyprland_frame_src_class_init(GstHyprlandFrameSrcClass* klass) {
    GObjectClass* gobject_class = G_OBJECT_CLASS(klass);
    GstElementClass* element_class = GST_ELEMENT_CLASS(klass);
    GstBaseSrcClass* basesrc_class = GST_BASE_SRC_CLASS(klass);

    gobject_class->set_property = gst_hyprland_frame_src_set_property;
    gobject_class->get_property = gst_hyprland_frame_src_get_property;
    gobject_class->finalize = gst_hyprland_frame_src_finalize;

    basesrc_class->get_caps = gst_hyprland_frame_src_get_caps;
    basesrc_class->start = gst_hyprland_frame_src_start;
    basesrc_class->stop = gst_hyprland_frame_src_stop;
    basesrc_class->create = gst_hyprland_frame_src_create;

    // Properties
    g_object_class_install_property(gobject_class, PROP_WIDTH,
        g_param_spec_uint("width", "Width", "Video width",
                         1, G_MAXUINT, DEFAULT_WIDTH,
                         (GParamFlags)(G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));

    g_object_class_install_property(gobject_class, PROP_HEIGHT,
        g_param_spec_uint("height", "Height", "Video height",
                         1, G_MAXUINT, DEFAULT_HEIGHT,
                         (GParamFlags)(G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));

    g_object_class_install_property(gobject_class, PROP_FRAMERATE,
        gst_param_spec_fraction("framerate", "Framerate", "Video framerate",
                               1, 1, 120, 1, DEFAULT_FRAMERATE_NUM, DEFAULT_FRAMERATE_DEN,
                               (GParamFlags)(G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));

    g_object_class_install_property(gobject_class, PROP_FORMAT,
        g_param_spec_string("format", "Format", "Video format",
                           DEFAULT_FORMAT,
                           (GParamFlags)(G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));

    // Element metadata
    gst_element_class_set_static_metadata(element_class,
        "Hyprland Frame Source",
        "Source/Video",
        "Captures frames from Hyprland compositor",
        "Hyprland Moonlight Integration");

    // Pad template
    GstCaps* caps = gst_caps_new_simple("video/x-raw",
        "format", G_TYPE_STRING, "RGBx",
        "width", GST_TYPE_INT_RANGE, 1, G_MAXINT,
        "height", GST_TYPE_INT_RANGE, 1, G_MAXINT,
        "framerate", GST_TYPE_FRACTION_RANGE, 1, 1, 120, 1,
        NULL);

    GstPadTemplate* src_template = gst_pad_template_new("src", GST_PAD_SRC, GST_PAD_ALWAYS, caps);
    gst_element_class_add_pad_template(element_class, src_template);
    gst_caps_unref(caps);
}

static void gst_hyprland_frame_src_init(GstHyprlandFrameSrc* src) {
    src->width = DEFAULT_WIDTH;
    src->height = DEFAULT_HEIGHT;
    src->framerate_num = DEFAULT_FRAMERATE_NUM;
    src->framerate_den = DEFAULT_FRAMERATE_DEN;
    src->format = g_strdup(DEFAULT_FORMAT);
    
    src->started = FALSE;
    src->timestamp = 0;
    src->frame_count = 0;
    
    g_mutex_init(&src->buffer_mutex);
    g_cond_init(&src->buffer_cond);
    src->current_buffer = NULL;
    src->new_frame_available = FALSE;
    src->monitor = NULL;

    // Set live source properties
    gst_base_src_set_live(GST_BASE_SRC(src), TRUE);
    gst_base_src_set_format(GST_BASE_SRC(src), GST_FORMAT_TIME);
}

static void gst_hyprland_frame_src_finalize(GObject* object) {
    GstHyprlandFrameSrc* src = GST_HYPRLAND_FRAME_SRC(object);
    
    g_free(src->format);
    
    g_mutex_clear(&src->buffer_mutex);
    g_cond_clear(&src->buffer_cond);
    
    if (src->current_buffer) {
        gst_buffer_unref(src->current_buffer);
        src->current_buffer = NULL;
    }

    G_OBJECT_CLASS(gst_hyprland_frame_src_parent_class)->finalize(object);
}

static void gst_hyprland_frame_src_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
    GstHyprlandFrameSrc* src = GST_HYPRLAND_FRAME_SRC(object);

    switch (prop_id) {
        case PROP_WIDTH:
            src->width = g_value_get_uint(value);
            break;
        case PROP_HEIGHT:
            src->height = g_value_get_uint(value);
            break;
        case PROP_FRAMERATE:
            src->framerate_num = gst_value_get_fraction_numerator(value);
            src->framerate_den = gst_value_get_fraction_denominator(value);
            break;
        case PROP_FORMAT:
            g_free(src->format);
            src->format = g_value_dup_string(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}

static void gst_hyprland_frame_src_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
    GstHyprlandFrameSrc* src = GST_HYPRLAND_FRAME_SRC(object);

    switch (prop_id) {
        case PROP_WIDTH:
            g_value_set_uint(value, src->width);
            break;
        case PROP_HEIGHT:
            g_value_set_uint(value, src->height);
            break;
        case PROP_FRAMERATE:
            gst_value_set_fraction(value, src->framerate_num, src->framerate_den);
            break;
        case PROP_FORMAT:
            g_value_set_string(value, src->format);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}

static GstCaps* gst_hyprland_frame_src_get_caps(GstBaseSrc* basesrc, GstCaps* filter) {
    GstHyprlandFrameSrc* src = GST_HYPRLAND_FRAME_SRC(basesrc);
    
    GstCaps* caps = gst_caps_new_simple("video/x-raw",
        "format", G_TYPE_STRING, src->format,
        "width", G_TYPE_INT, (gint)src->width,
        "height", G_TYPE_INT, (gint)src->height,
        "framerate", GST_TYPE_FRACTION, (gint)src->framerate_num, (gint)src->framerate_den,
        NULL);

    if (filter) {
        GstCaps* intersection = gst_caps_intersect_full(filter, caps, GST_CAPS_INTERSECT_FIRST);
        gst_caps_unref(caps);
        caps = intersection;
    }

    return caps;
}

static gboolean gst_hyprland_frame_src_start(GstBaseSrc* basesrc) {
    GstHyprlandFrameSrc* src = GST_HYPRLAND_FRAME_SRC(basesrc);
    
    src->started = TRUE;
    src->timestamp = 0;
    src->frame_count = 0;
    
    Debug::log(LOG, "HyprlandFrameSource: Started ({}x{} @ {}/{} fps)", 
              src->width, src->height, src->framerate_num, src->framerate_den);
    
    return TRUE;
}

static gboolean gst_hyprland_frame_src_stop(GstBaseSrc* basesrc) {
    GstHyprlandFrameSrc* src = GST_HYPRLAND_FRAME_SRC(basesrc);
    
    src->started = FALSE;
    
    g_mutex_lock(&src->buffer_mutex);
    if (src->current_buffer) {
        gst_buffer_unref(src->current_buffer);
        src->current_buffer = NULL;
    }
    src->new_frame_available = FALSE;
    g_mutex_unlock(&src->buffer_mutex);
    
    Debug::log(LOG, "HyprlandFrameSource: Stopped");
    
    return TRUE;
}

static GstFlowReturn gst_hyprland_frame_src_create(GstBaseSrc* basesrc, guint64 offset, guint size, GstBuffer** buffer) {
    GstHyprlandFrameSrc* src = GST_HYPRLAND_FRAME_SRC(basesrc);
    
    if (!src->started) {
        return GST_FLOW_FLUSHING;
    }
    
    // Wait for new frame
    g_mutex_lock(&src->buffer_mutex);
    while (!src->new_frame_available && src->started) {
        g_cond_wait(&src->buffer_cond, &src->buffer_mutex);
    }
    
    if (!src->started) {
        g_mutex_unlock(&src->buffer_mutex);
        return GST_FLOW_FLUSHING;
    }
    
    if (!src->current_buffer) {
        g_mutex_unlock(&src->buffer_mutex);
        return GST_FLOW_ERROR;
    }
    
    // Take ownership of current buffer
    *buffer = src->current_buffer;
    src->current_buffer = NULL;
    src->new_frame_available = FALSE;
    
    g_mutex_unlock(&src->buffer_mutex);
    
    // Set timestamp
    GST_BUFFER_PTS(*buffer) = src->timestamp;
    GST_BUFFER_DTS(*buffer) = src->timestamp;
    GST_BUFFER_DURATION(*buffer) = gst_util_uint64_scale(GST_SECOND, src->framerate_den, src->framerate_num);
    
    src->timestamp += GST_BUFFER_DURATION(*buffer);
    src->frame_count++;
    
    return GST_FLOW_OK;
}

// Public API: Push buffer from Hyprland
void gst_hyprland_frame_src_push_buffer(GstHyprlandFrameSrc* src, SP<Aquamarine::IBuffer> aq_buffer) {
    if (!src->started || !aq_buffer) {
        return;
    }
    
    // Convert Aquamarine buffer to GStreamer buffer
    GstBuffer* gst_buffer = aquamarine_buffer_to_gst_buffer(aq_buffer, src->width, src->height);
    if (!gst_buffer) {
        Debug::log(ERR, "HyprlandFrameSource: Failed to convert buffer");
        return;
    }
    
    g_mutex_lock(&src->buffer_mutex);
    
    // Replace current buffer
    if (src->current_buffer) {
        gst_buffer_unref(src->current_buffer);
    }
    
    src->current_buffer = gst_buffer;
    src->new_frame_available = TRUE;
    g_cond_signal(&src->buffer_cond);
    
    g_mutex_unlock(&src->buffer_mutex);
}

// Helper: Convert Aquamarine buffer to GStreamer buffer
static GstBuffer* aquamarine_buffer_to_gst_buffer(SP<Aquamarine::IBuffer> aq_buffer, guint width, guint height) {
    if (!aq_buffer) {
        return NULL;
    }
    
    // Get buffer attributes
    auto attrs = aq_buffer->dmabuf();
    if (!attrs.success) {
        Debug::log(ERR, "HyprlandFrameSource: Failed to get DMA-BUF attributes");
        return NULL;
    }
    
    // For now, do a simple memory copy approach
    // TODO: Implement zero-copy DMA-BUF sharing
    
    // Calculate buffer size (assuming RGBx format, 4 bytes per pixel)
    gsize buffer_size = width * height * 4;
    
    // Create GStreamer buffer
    GstBuffer* gst_buffer = gst_buffer_new_allocate(NULL, buffer_size, NULL);
    if (!gst_buffer) {
        Debug::log(ERR, "HyprlandFrameSource: Failed to allocate GStreamer buffer");
        return NULL;
    }
    
    // Map and copy data
    GstMapInfo map;
    if (gst_buffer_map(gst_buffer, &map, GST_MAP_WRITE)) {
        // Here we would normally copy the pixel data from the Aquamarine buffer
        // For now, we'll create a test pattern since the actual copy implementation
        // depends on the specific buffer format and layout
        
        // TODO: Implement actual pixel data copy from Aquamarine buffer
        // This would involve:
        // 1. Mapping the DMA-BUF for reading
        // 2. Converting pixel format if necessary
        // 3. Copying scanlines with proper stride handling
        
        // Placeholder: Fill with test pattern
        memset(map.data, 0x80, map.size); // Gray fill for now
        
        gst_buffer_unmap(gst_buffer, &map);
    } else {
        Debug::log(ERR, "HyprlandFrameSource: Failed to map GStreamer buffer");
        gst_buffer_unref(gst_buffer);
        return NULL;
    }
    
    return gst_buffer;
}

// Helper: Convert DRM format to GStreamer format
static GstVideoFormat drm_format_to_gst_format(uint32_t drm_format) {
    switch (drm_format) {
        case DRM_FORMAT_XRGB8888:
        case DRM_FORMAT_ARGB8888:
            return GST_VIDEO_FORMAT_RGBx;
        case DRM_FORMAT_XBGR8888:
        case DRM_FORMAT_ABGR8888:
            return GST_VIDEO_FORMAT_BGRx;
        default:
            Debug::log(WARN, "HyprlandFrameSource: Unsupported DRM format: {}", drm_format);
            return GST_VIDEO_FORMAT_RGBx; // Fallback
    }
}

// Plugin registration
gboolean gst_hyprland_frame_src_plugin_init(GstPlugin* plugin) {
    GST_DEBUG_CATEGORY_INIT(gst_hyprland_frame_src_debug, "hyprlandframesrc", 0, "Hyprland Frame Source");
    
    return gst_element_register(plugin, "hyprlandframesrc", GST_RANK_NONE, GST_TYPE_HYPRLAND_FRAME_SRC);
}