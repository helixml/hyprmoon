#!/bin/bash
set -e

echo "🔍 Debugging crash report directory and user permissions"

# Check current user and permissions
echo "📋 Current user and environment:"
echo "User: $(whoami)"
echo "UID: $(id -u)"
echo "GID: $(id -g)"
echo "Groups: $(groups)"
echo "HOME: $HOME"
echo "PWD: $(pwd)"

# Check where Hyprland tries to create crash reports
echo ""
echo "📋 Checking Hyprland crash report behavior:"
echo "XDG_RUNTIME_DIR: $XDG_RUNTIME_DIR"
echo "XDG_CACHE_HOME: ${XDG_CACHE_HOME:-not set}"
echo "XDG_DATA_HOME: ${XDG_DATA_HOME:-not set}"

# Create potential crash report directories
echo ""
echo "📋 Creating potential crash report directories:"
mkdir -p /tmp/hypr-crash || echo "Failed to create /tmp/hypr-crash"
mkdir -p "$HOME/.cache/hypr" || echo "Failed to create $HOME/.cache/hypr"
mkdir -p "$XDG_RUNTIME_DIR/hypr" || echo "Failed to create $XDG_RUNTIME_DIR/hypr"
mkdir -p /var/crash || echo "Failed to create /var/crash"

# Set permissions
chmod 777 /tmp/hypr-crash 2>/dev/null || true
chmod 777 "$XDG_RUNTIME_DIR/hypr" 2>/dev/null || true

echo ""
echo "📋 Directory permissions:"
ls -la /tmp/ | grep hypr || echo "No hypr dirs in /tmp"
ls -la "$XDG_RUNTIME_DIR/" | grep hypr || echo "No hypr dirs in $XDG_RUNTIME_DIR"

# Setup environment
export XDG_RUNTIME_DIR=/tmp/runtime-root
mkdir -p $XDG_RUNTIME_DIR
chmod 700 $XDG_RUNTIME_DIR

# Try with debug environment variables
echo ""
echo "📋 Testing with crash report directories created:"
export WLR_BACKENDS=headless
export WLR_RENDERER=pixman
export LIBGL_ALWAYS_SOFTWARE=1
export HYPRLAND_LOG_WLR=1

# Create more potential crash directories
export XDG_CACHE_HOME=/tmp/cache
export XDG_DATA_HOME=/tmp/data
mkdir -p "$XDG_CACHE_HOME/hypr"
mkdir -p "$XDG_DATA_HOME/hypr"
chmod 777 "$XDG_CACHE_HOME/hypr"
chmod 777 "$XDG_DATA_HOME/hypr"

echo "--- Hyprland Output with Crash Directories Fixed ---"
timeout 15 /usr/bin/Hyprland --config /test_config/hyprland.conf --i-am-really-stupid 2>&1 || true

echo ""
echo "📋 Check if any crash files were created:"
find /tmp -name "*hypr*" -o -name "*crash*" 2>/dev/null || echo "No crash files found"
find "$XDG_RUNTIME_DIR" -name "*crash*" 2>/dev/null || echo "No crash files in runtime dir"

echo ""
echo "📋 ACTUAL CRASH REPORT CONTENT:"
if [ -f "/tmp/cache/hyprland/hyprlandCrashReport62.txt" ]; then
    echo "--- Crash Report Content ---"
    cat "/tmp/cache/hyprland/hyprlandCrashReport62.txt"
    echo "--- End Crash Report ---"
else
    echo "Finding latest crash report..."
    LATEST_CRASH=$(find /tmp -name "*hyprlandCrashReport*.txt" 2>/dev/null | head -1)
    if [ -n "$LATEST_CRASH" ]; then
        echo "--- Latest Crash Report: $LATEST_CRASH ---"
        cat "$LATEST_CRASH"
        echo "--- End Crash Report ---"
    else
        echo "❌ No crash report files found"
    fi
fi

echo ""
echo "📋 Hyprland log files:"
find /tmp -name "hyprland.log" 2>/dev/null | while read logfile; do
    echo "--- Log: $logfile ---"
    tail -20 "$logfile"
    echo "--- End Log ---"
done

echo "🏁 Crash report debugging complete"