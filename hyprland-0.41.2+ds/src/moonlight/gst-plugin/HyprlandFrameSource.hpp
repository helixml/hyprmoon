#pragma once

#include <gst/gst.h>
#include <gst/base/gstbasesrc.h>
#include <gst/video/video.h>
#include <wlr/interfaces/wlr_buffer.h>
#include "helpers/Monitor.hpp"

G_BEGIN_DECLS

#define GST_TYPE_HYPRLAND_FRAME_SRC (gst_hyprland_frame_src_get_type())
#define GST_HYPRLAND_FRAME_SRC(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_HYPRLAND_FRAME_SRC, GstHyprlandFrameSrc))
#define GST_HYPRLAND_FRAME_SRC_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_HYPRLAND_FRAME_SRC, GstHyprlandFrameSrcClass))
#define GST_IS_HYPRLAND_FRAME_SRC(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_HYPRLAND_FRAME_SRC))
#define GST_IS_HYPRLAND_FRAME_SRC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_HYPRLAND_FRAME_SRC))

typedef struct _GstHyprlandFrameSrc GstHyprlandFrameSrc;
typedef struct _GstHyprlandFrameSrcClass GstHyprlandFrameSrcClass;

struct _GstHyprlandFrameSrc {
    GstBaseSrc parent;

    // Properties
    guint width;
    guint height;
    guint framerate_num;
    guint framerate_den;
    gchar* format;
    
    // State
    gboolean started;
    GstClockTime timestamp;
    guint64 frame_count;
    
    // Buffer management
    GMutex buffer_mutex;
    GCond buffer_cond;
    GstBuffer* current_buffer;
    gboolean new_frame_available;
    
    // Monitor tracking
    gpointer monitor; // PHLMONITOR (void* to avoid header deps)
};

struct _GstHyprlandFrameSrcClass {
    GstBaseSrcClass parent_class;
};

// GObject type registration
GType gst_hyprland_frame_src_get_type(void);

// Public API for pushing frames from Hyprland
void gst_hyprland_frame_src_push_buffer(GstHyprlandFrameSrc* src, wlr_buffer* buffer);

// Registration function
gboolean gst_hyprland_frame_src_plugin_init(GstPlugin* plugin);

G_END_DECLS