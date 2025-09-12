#!/bin/bash
set -e

echo "🔍 Debugging container environment for Hyprland startup"

# Check DRI devices
echo "📱 DRI devices:"
ls -la /dev/dri/ || echo "No DRI devices found"

# Check OpenGL/Mesa
echo "🎨 OpenGL info:"
glxinfo 2>&1 | head -10 || echo "glxinfo not available"

# Check environment
echo "🌍 Environment variables:"
env | grep -E "(WLR|GL|NVIDIA|EGL|GBM|GPU)" | sort

# Test EGL directly
echo "🖥️ Testing EGL:"
cat > /tmp/test_egl.c << 'EOF'
#include <EGL/egl.h>
#include <stdio.h>

int main() {
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        printf("❌ Failed to get EGL display\n");
        return 1;
    }
    
    EGLint major, minor;
    if (!eglInitialize(display, &major, &minor)) {
        printf("❌ Failed to initialize EGL\n");
        return 1;
    }
    
    printf("✅ EGL initialized successfully: version %d.%d\n", major, minor);
    eglTerminate(display);
    return 0;
}
EOF

if gcc -o /tmp/test_egl /tmp/test_egl.c -lEGL 2>/dev/null; then
    /tmp/test_egl
else
    echo "❌ Failed to compile EGL test"
fi

# Try running Hyprland with verbose output
echo "🎮 Testing Hyprland with debug output:"
export WAYLAND_DEBUG=1
export HYPRLAND_LOG_WLR=1
export XDG_RUNTIME_DIR=/tmp/runtime-root
mkdir -p $XDG_RUNTIME_DIR
chmod 700 $XDG_RUNTIME_DIR

timeout 5 /usr/bin/Hyprland --config /test_config/hyprland.conf --help 2>&1 || echo "Hyprland help command completed"

echo "🔧 Attempting Hyprland startup with full debug..."
timeout 10 /usr/bin/Hyprland --config /test_config/hyprland.conf --i-am-really-stupid 2>&1 | head -20 || echo "Hyprland startup debug completed"

echo "🏁 Debug environment check completed"