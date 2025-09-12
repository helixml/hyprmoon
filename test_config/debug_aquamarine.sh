#!/bin/bash
set -e

echo "🔍 Deep debugging Aquamarine backend initialization"

# Setup environment
export XDG_RUNTIME_DIR=/tmp/runtime-root
mkdir -p $XDG_RUNTIME_DIR
chmod 700 $XDG_RUNTIME_DIR

# Set up same environment as E2E test
export WLR_RENDERER=gles2
export WLR_BACKENDS=headless
export WLR_NO_HARDWARE_CURSORS=1
export WLR_HEADLESS_OUTPUTS=1
export __GL_THREADED_OPTIMIZATIONS=1
export __GL_SYNC_TO_VBLANK=1
export LIBGL_ALWAYS_SOFTWARE=0
export NVIDIA_VISIBLE_DEVICES=all
export NVIDIA_DRIVER_CAPABILITIES=all
export EGL_PLATFORM=drm
export GBM_BACKEND=nvidia-drm
export __GLX_VENDOR_LIBRARY_NAME=nvidia
export WLR_DRM_DEVICES=/dev/dri/card0
export WLR_RENDERER_ALLOW_SOFTWARE=1
export WLR_DRM_NO_ATOMIC=1
export GPU_RENDER_NODE=/dev/dri/renderD128
export GST_GL_DRM_DEVICE=/dev/dri/renderD128
export GST_GL_API=gles2
export GST_GL_WINDOW=surfaceless

echo "📱 Detailed DRI device analysis:"
ls -la /dev/dri/
echo ""
echo "Device permissions:"
for dev in /dev/dri/*; do
    echo "  $(ls -la $dev) - $(file $dev)"
done

echo ""
echo "🔗 Library path analysis:"
echo "LIBGL_DRIVERS_PATH locations:"
for path in /usr/lib/dri /usr/lib/x86_64-linux-gnu/dri /usr/lib64/dri; do
    if [ -d "$path" ]; then
        echo "  $path: $(ls $path 2>/dev/null | grep -E '(swrast|nouveau|radeon|intel|nvidia)' | head -3)"
    else
        echo "  $path: not found"
    fi
done

echo ""
echo "🖥️ Testing direct EGL/GBM access:"
cat > /tmp/test_gbm.c << 'EOF'
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <gbm.h>
#include <EGL/egl.h>

int main() {
    printf("Testing GBM device access...\n");
    
    // Test card0
    int fd = open("/dev/dri/card0", O_RDWR);
    if (fd < 0) {
        printf("❌ Failed to open /dev/dri/card0\n");
        return 1;
    }
    
    struct gbm_device *gbm = gbm_create_device(fd);
    if (!gbm) {
        printf("❌ Failed to create GBM device\n");
        close(fd);
        return 1;
    }
    
    printf("✅ GBM device created successfully\n");
    
    // Test EGL
    EGLDisplay display = eglGetPlatformDisplay(EGL_PLATFORM_GBM_MESA, gbm, NULL);
    if (display == EGL_NO_DISPLAY) {
        printf("❌ Failed to get EGL display\n");
        gbm_device_destroy(gbm);
        close(fd);
        return 1;
    }
    
    EGLint major, minor;
    if (!eglInitialize(display, &major, &minor)) {
        printf("❌ Failed to initialize EGL\n");
        gbm_device_destroy(gbm);
        close(fd);
        return 1;
    }
    
    printf("✅ EGL initialized: %d.%d\n", major, minor);
    
    eglTerminate(display);
    gbm_device_destroy(gbm);
    close(fd);
    return 0;
}
EOF

if gcc -o /tmp/test_gbm /tmp/test_gbm.c -lgbm -lEGL 2>/dev/null; then
    chmod +x /tmp/test_gbm
    /tmp/test_gbm
else
    echo "❌ Failed to compile GBM test"
fi

echo ""
echo "🌊 Testing Aquamarine backends specifically:"

# Check what Aquamarine backends are available
echo "Available Aquamarine backend types:"
strings /usr/bin/Hyprland | grep -i "backend\|aquamarine" | head -10

echo ""
echo "🎮 Testing Hyprland with different backend environment:"

# Try with software rendering first
echo "Testing with software rendering..."
export WLR_RENDERER=pixman
export LIBGL_ALWAYS_SOFTWARE=1
timeout 5 strace -e openat,access /usr/bin/Hyprland --config /test_config/hyprland.conf --i-am-really-stupid 2>&1 | grep -E "(drm|gbm|egl|aquamarine|backend)" | head -20

echo ""
echo "Testing with original GPU settings..."
export WLR_RENDERER=gles2  
export LIBGL_ALWAYS_SOFTWARE=0
timeout 5 strace -e openat,access /usr/bin/Hyprland --config /test_config/hyprland.conf --i-am-really-stupid 2>&1 | grep -E "(drm|gbm|egl|aquamarine|backend)" | head -20

echo ""
echo "🔧 Checking Hyprland configuration:"
echo "Hyprland config file:"
cat /test_config/hyprland.conf

echo ""
echo "🏁 Aquamarine debug analysis complete"