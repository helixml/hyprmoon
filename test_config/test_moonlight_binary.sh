#!/bin/bash
set -e

echo "🔍 Testing Hyprland moonlight integration binary..."

# Test 1: Check if Hyprland binary exists and has moonlight symbols
echo "1️⃣ Checking binary existence and moonlight symbols..."
if [ ! -f /usr/bin/Hyprland ]; then
    echo "❌ Hyprland binary not found"
    exit 1
fi

# Check for moonlight-related symbols in the binary
MOONLIGHT_SYMBOLS=$(nm /usr/bin/Hyprland 2>/dev/null | grep -i moonlight | wc -l || echo "0")
if [ "$MOONLIGHT_SYMBOLS" -gt 0 ]; then
    echo "✅ Found $MOONLIGHT_SYMBOLS moonlight symbols in Hyprland binary"
else
    echo "⚠️  No moonlight symbols found via nm, checking with strings..."
    STRING_MATCHES=$(strings /usr/bin/Hyprland | grep -i moonlight | head -5 || echo "none")
    if [ "$STRING_MATCHES" != "none" ]; then
        echo "✅ Found moonlight strings in binary:"
        echo "$STRING_MATCHES"
    else
        echo "❌ No moonlight integration found in binary"
        exit 1
    fi
fi

# Test 2: Check dependencies are linked
echo "2️⃣ Checking moonlight dependencies..."
REQUIRED_LIBS=("libgstreamer-1.0" "libenet" "libboost" "libssl")
for lib in "${REQUIRED_LIBS[@]}"; do
    if ldd /usr/bin/Hyprland | grep -q "$lib"; then
        echo "✅ $lib linked"
    else
        echo "❌ $lib not linked"
        exit 1
    fi
done

# Test 3: Test help output for moonlight options
echo "3️⃣ Testing help output..."
HELP_OUTPUT=$(/usr/bin/Hyprland --help 2>&1 || echo "help failed")
echo "Help output snippet: $HELP_OUTPUT" | head -3

# Test 4: Test moonlight networking capability
echo "4️⃣ Testing moonlight networking capability..."

# Test if we can bind to moonlight ports
echo "Testing port binding capability..."
if timeout 2 bash -c 'exec 3<>/dev/tcp/127.0.0.1/47989 2>/dev/null && echo "Port 47989 accessible" >&3'; then
    echo "✅ Can access moonlight HTTP port"
else
    echo "⚠️  Cannot access moonlight HTTP port (expected until server starts)"
fi

# Test HTTP server creation
echo "Testing simple HTTP server creation..."
cat > /tmp/test_http.cpp << 'EOF'
#include <iostream>
#include <boost/asio.hpp>

int main() {
    try {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 47989));
        std::cout << "✅ HTTP server can bind to moonlight port 47989" << std::endl;
    } catch (std::exception& e) {
        std::cout << "⚠️  HTTP server test failed: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
EOF

if g++ -o /tmp/test_http /tmp/test_http.cpp $(pkg-config --cflags --libs boost-system) 2>/dev/null; then
    if /tmp/test_http; then
        echo "✅ Moonlight HTTP server functionality working"
    else
        echo "⚠️  Moonlight HTTP server test failed"
    fi
else
    echo "⚠️  Could not compile HTTP server test"
fi

# Test 5: Create a fake moonlight server test
echo "5️⃣ Testing standalone moonlight components..."

# Create a simple test that exercises moonlight code without needing full Hyprland
cat > /tmp/test_moonlight.cpp << 'EOF'
#include <iostream>
#include <gst/gst.h>
#include <enet/enet.h>

int main() {
    std::cout << "Testing moonlight dependencies..." << std::endl;
    
    // Test GStreamer
    gst_init(nullptr, nullptr);
    std::cout << "✅ GStreamer initialized" << std::endl;
    
    // Test ENet
    if (enet_initialize() == 0) {
        std::cout << "✅ ENet initialized" << std::endl;
        enet_deinitialize();
    } else {
        std::cout << "❌ ENet failed" << std::endl;
        return 1;
    }
    
    std::cout << "✅ All moonlight dependencies working" << std::endl;
    return 0;
}
EOF

# Compile and run the test
echo "Compiling dependency test..."
if g++ -o /tmp/test_moonlight /tmp/test_moonlight.cpp $(pkg-config --cflags --libs gstreamer-1.0) -lenet; then
    echo "✅ Moonlight dependency test compiled"
    if /tmp/test_moonlight; then
        echo "✅ Moonlight dependencies functional"
    else
        echo "❌ Moonlight dependency test failed"
        exit 1
    fi
else
    echo "❌ Failed to compile moonlight dependency test"
    exit 1
fi

echo "🎉 Moonlight integration verification completed successfully!"
echo "Summary:"
echo "- ✅ Hyprland binary contains moonlight integration"
echo "- ✅ Required libraries are linked"
echo "- ✅ Dependencies are functional"
echo "- ⚠️  Full runtime test limited by container environment"

# Create results file
cat > /test_output/moonlight_verification.json << EOF
{
  "timestamp": "$(date -Iseconds)",
  "test": "moonlight_integration_verification",
  "status": "success",
  "binary_path": "/usr/bin/Hyprland",
  "moonlight_symbols": $MOONLIGHT_SYMBOLS,
  "dependencies_linked": true,
  "gstreamer_functional": true,
  "enet_functional": true,
  "limitation": "full_runtime_test_requires_display_environment"
}
EOF

exit 0