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

# Create standard cache directories and ensure they're bind-mounted
export XDG_CACHE_HOME=/tmp/cache
export XDG_DATA_HOME=/tmp/data
mkdir -p "$XDG_CACHE_HOME/hyprland"
mkdir -p "$XDG_DATA_HOME/hypr"
mkdir -p "/test_output/crash_reports"
chmod 777 "$XDG_CACHE_HOME/hyprland"
chmod 777 "$XDG_DATA_HOME/hypr"
chmod 777 "/test_output/crash_reports"

# Enable debug logging environment variables
echo ""
echo "📋 Enabling debug logging:"
export WLR_BACKENDS=headless
export WLR_RENDERER=pixman
export LIBGL_ALWAYS_SOFTWARE=1
export HYPRLAND_LOG_WLR=1
export WLR_DEBUG=1
export WAYLAND_DEBUG=1

# Check if Hyprland has debug build flags or verbose options
echo "--- Checking Hyprland debug options ---"
/usr/bin/Hyprland --help 2>&1 | grep -i -E "(debug|verbose|log)" || echo "No obvious debug flags found"

echo ""
echo "📋 Testing with crash report directories created and debug logging enabled:"
echo "--- Hyprland Output with Full Debug ---"
timeout 15 /usr/bin/Hyprland --config /test_config/hyprland.conf --i-am-really-stupid 2>&1 || true

echo ""
echo "📋 Finding and copying crash reports (sorted by timestamp):"

# Find all crash reports and sort by modification time (latest first)
find /tmp -name "*hyprlandCrashReport*.txt" -printf "%T@ %p\n" 2>/dev/null | sort -nr | while read timestamp filepath; do
    if [ -f "$filepath" ]; then
        filename=$(basename "$filepath")
        echo "📄 Found crash report: $filepath (timestamp: $timestamp)"
        
        # Copy to test_output for user access
        cp "$filepath" "/test_output/crash_reports/${filename}"
        echo "   ✅ Copied to /test_output/crash_reports/${filename}"
    fi
done

# Also find crash reports in other locations
find "$HOME" -name "*hyprlandCrashReport*.txt" -printf "%T@ %p\n" 2>/dev/null | sort -nr | while read timestamp filepath; do
    if [ -f "$filepath" ]; then
        filename=$(basename "$filepath")
        echo "📄 Found crash report: $filepath (timestamp: $timestamp)"
        cp "$filepath" "/test_output/crash_reports/${filename}"
        echo "   ✅ Copied to /test_output/crash_reports/${filename}"
    fi
done

echo ""
echo "📋 LATEST CRASH REPORT CONTENT:"
# Get the most recent crash report by modification time
LATEST_CRASH=$(find /tmp "$HOME" -name "*hyprlandCrashReport*.txt" -printf "%T@ %p\n" 2>/dev/null | sort -nr | head -1 | cut -d' ' -f2-)

if [ -n "$LATEST_CRASH" ] && [ -f "$LATEST_CRASH" ]; then
    echo "--- Latest Crash Report: $LATEST_CRASH ---"
    cat "$LATEST_CRASH"
    echo "--- End Crash Report ---"
    
    # Also copy this one with a special name
    cp "$LATEST_CRASH" "/test_output/crash_reports/LATEST_CRASH_REPORT.txt"
    echo "   ✅ Latest crash report copied to /test_output/crash_reports/LATEST_CRASH_REPORT.txt"
else
    echo "❌ No crash report files found"
fi

echo ""
echo "📋 Hyprland log files (sorted by timestamp):"
find /tmp "$XDG_RUNTIME_DIR" -name "hyprland.log" -printf "%T@ %p\n" 2>/dev/null | sort -nr | while read timestamp logfile; do
    if [ -f "$logfile" ]; then
        echo "--- Log: $logfile (timestamp: $timestamp) ---"
        tail -50 "$logfile"
        echo "--- End Log ---"
        
        # Copy log to test_output
        logname=$(basename "$(dirname "$logfile")")_hyprland.log
        cp "$logfile" "/test_output/crash_reports/${logname}"
        echo "   ✅ Log copied to /test_output/crash_reports/${logname}"
    fi
done

echo ""
echo "📋 Final crash report directory contents:"
ls -la "/test_output/crash_reports/" || echo "No crash reports directory"

echo "🏁 Crash report debugging complete"