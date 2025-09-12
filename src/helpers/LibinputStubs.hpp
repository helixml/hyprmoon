#pragma once

#ifdef NO_LIBINPUT

// Stub functions for libinput when building without it
inline bool wlr_input_device_is_libinput(const void* device) {
    return false;
}

inline void* wlr_libinput_get_device_handle(const void* device) {
    return nullptr;
}

// Define empty macros for libinput constants
#define LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS 0
#define LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER 1
#define LIBINPUT_CONFIG_MIDDLE_EMULATION_ENABLED 1
#define LIBINPUT_CONFIG_MIDDLE_EMULATION_DISABLED 0
#define LIBINPUT_CONFIG_TAP_MAP_LRM 0
#define LIBINPUT_CONFIG_TAP_MAP_LMR 1
#define LIBINPUT_CONFIG_SCROLL_NO_SCROLL 0
#define LIBINPUT_CONFIG_SCROLL_2FG 1
#define LIBINPUT_CONFIG_SCROLL_EDGE 2
#define LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN 3
#define LIBINPUT_CONFIG_DRAG_DISABLED 0
#define LIBINPUT_CONFIG_DRAG_ENABLED 1
#define LIBINPUT_CONFIG_DRAG_LOCK_DISABLED 0
#define LIBINPUT_CONFIG_DRAG_LOCK_ENABLED 1
#define LIBINPUT_CONFIG_TAP_ENABLED 1
#define LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE 1
#define LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT 2
#define LIBINPUT_CONFIG_ACCEL_PROFILE_CUSTOM 3
#define LIBINPUT_CONFIG_SCROLL_BUTTON_LOCK_ENABLED 1
#define LIBINPUT_CONFIG_SCROLL_BUTTON_LOCK_DISABLED 0
#define LIBINPUT_DEVICE_CAP_POINTER 1
#define LIBINPUT_ACCEL_TYPE_SCROLL 1
#define LIBINPUT_ACCEL_TYPE_MOTION 2

// Stub functions
inline double libinput_device_config_accel_get_speed(void*) { return 0.0; }
inline double libinput_device_config_accel_get_default_speed(void*) { return 0.0; }
inline int libinput_device_config_accel_get_profile(void*) { return 0; }
inline int libinput_device_config_accel_get_default_profile(void*) { return 0; }
inline int libinput_device_has_capability(void*, int) { return 0; }
inline int libinput_device_get_size(void*, double*, double*) { return -1; }
inline int libinput_device_config_click_set_method(void*, int) { return 0; }
inline int libinput_device_config_left_handed_set(void*, int) { return 0; }
inline int libinput_device_config_middle_emulation_is_available(void*) { return 0; }
inline int libinput_device_config_middle_emulation_set_enabled(void*, int) { return 0; }
inline int libinput_device_config_tap_set_button_map(void*, int) { return 0; }
inline int libinput_device_config_scroll_set_method(void*, int) { return 0; }
inline int libinput_device_config_scroll_get_default_method(void*) { return 0; }
inline int libinput_device_config_tap_set_drag_enabled(void*, int) { return 0; }
inline int libinput_device_config_tap_set_drag_lock_enabled(void*, int) { return 0; }
inline int libinput_device_config_tap_get_finger_count(void*) { return 0; }
inline int libinput_device_config_tap_set_enabled(void*, int) { return 0; }
inline int libinput_device_config_scroll_has_natural_scroll(void*) { return 0; }
inline int libinput_device_config_scroll_set_natural_scroll_enabled(void*, int) { return 0; }
inline int libinput_device_config_dwt_is_available(void*) { return 0; }
inline int libinput_device_config_dwt_set_enabled(void*, int) { return 0; }
inline int libinput_device_config_accel_set_speed(void*, double) { return 0; }
inline int libinput_device_config_accel_set_profile(void*, int) { return 0; }
inline int libinput_device_config_scroll_set_button(void*, int) { return 0; }
inline int libinput_device_config_scroll_get_default_button(void*) { return 0; }
inline int libinput_device_config_scroll_set_button_lock(void*, int) { return 0; }
inline int libinput_device_config_send_events_get_mode(void*) { return 0; }
inline int libinput_device_config_send_events_set_mode(void*, int) { return 0; }
inline int libinput_device_config_calibration_has_matrix(void*) { return 0; }
inline int libinput_device_config_calibration_set_matrix(void*, const float*) { return 0; }

// Stub config types
struct libinput_config_accel;
typedef struct libinput_config_accel* libinput_config_accel_t;
inline libinput_config_accel_t libinput_config_accel_create(int) { return nullptr; }
inline void libinput_config_accel_destroy(libinput_config_accel_t) {}
inline int libinput_config_accel_set_points(libinput_config_accel_t, int, double, size_t, const double*) { return 0; }
inline int libinput_device_config_accel_apply(void*, libinput_config_accel_t) { return 0; }

#endif // NO_LIBINPUT