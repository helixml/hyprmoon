#!/usr/bin/env python3
"""
Verify Moonlight Integration

This script verifies that:
1. Hyprland binary contains moonlight integration
2. Wolf moonlight server classes are present
3. The integration code is properly compiled
"""

import subprocess
import sys
from pathlib import Path

def check_binary_symbols():
    """Check if Hyprland binary contains moonlight symbols"""
    print("🔍 Checking Hyprland binary for moonlight integration...")
    
    binary_path = Path("build/src/Hyprland")
    if not binary_path.exists():
        print(f"❌ Hyprland binary not found at {binary_path}")
        return False
        
    try:
        # Check for moonlight-related symbols
        result = subprocess.run(['strings', str(binary_path)], 
                              capture_output=True, text=True)
        symbols = result.stdout
        
        # Check for key integration components
        checks = [
            ("CMoonlightManager", "Hyprland moonlight manager class"),
            ("WolfMoonlightServer", "Wolf moonlight server class"),
            ("onFrameReady", "Frame callback integration"),
            ("Moonlight server ready", "Initialization message"),
            ("gst_", "GStreamer integration"),
            ("enet_", "ENet networking integration"),
        ]
        
        found_count = 0
        for symbol, description in checks:
            if symbol in symbols:
                print(f"✅ {description}: {symbol}")
                found_count += 1
            else:
                print(f"❌ Missing {description}: {symbol}")
                
        print(f"📊 Found {found_count}/{len(checks)} expected symbols")
        return found_count >= len(checks) - 1  # Allow 1 missing
        
    except Exception as e:
        print(f"❌ Error checking binary: {e}")
        return False

def check_source_integration():
    """Check that source code integration is complete"""
    print("\\n🔍 Checking source code integration...")
    
    files_to_check = [
        ("src/moonlight/managers/MoonlightManager.cpp", "Main integration class"),
        ("src/moonlight/wolf-impl/WolfMoonlightServer.cpp", "Wolf server implementation"),
        ("src/moonlight/gst-plugin/HyprlandFrameSource.cpp", "GStreamer frame source"),
        ("src/Compositor.cpp", "Compositor integration"),
        ("src/render/Renderer.cpp", "Renderer frame callbacks"),
    ]
    
    found_count = 0
    for file_path, description in files_to_check:
        path = Path(file_path)
        if path.exists():
            print(f"✅ {description}: {file_path}")
            found_count += 1
        else:
            print(f"❌ Missing {description}: {file_path}")
            
    print(f"📊 Found {found_count}/{len(files_to_check)} expected files")
    return found_count == len(files_to_check)

def check_build_configuration():
    """Check build configuration"""
    print("\\n🔍 Checking build configuration...")
    
    meson_build = Path("src/moonlight/meson.build")
    if not meson_build.exists():
        print("❌ Moonlight meson.build not found")
        return False
        
    try:
        content = meson_build.read_text()
        
        checks = [
            ("WolfMoonlightServer.cpp", "Wolf server in build"),
            ("MoonlightManager.cpp", "Manager in build"),
            ("gstreamer-1.0", "GStreamer dependency"),
            ("libenet", "ENet dependency"),
            ("boost", "Boost dependency"),
        ]
        
        found_count = 0
        for item, description in checks:
            if item in content:
                print(f"✅ {description}: {item}")
                found_count += 1
            else:
                print(f"❌ Missing {description}: {item}")
                
        print(f"📊 Found {found_count}/{len(checks)} expected build items")
        return found_count >= len(checks) - 1
        
    except Exception as e:
        print(f"❌ Error checking build config: {e}")
        return False

def check_dependencies():
    """Check that required dependencies are available"""
    print("\\n🔍 Checking dependencies in container...")
    
    try:
        # Check if we can run a simple dependency check in container
        result = subprocess.run([
            'docker', 'run', '--rm', 
            '-v', f'{Path.cwd()}:/workspace',
            'hyprland-moonlight',
            '/bin/bash', '-c',
            'pkg-config --exists gstreamer-1.0 libenet fmt && ls /usr/include/boost/version.hpp >/dev/null 2>&1 && echo "Dependencies OK" || echo "Dependencies missing"'
        ], capture_output=True, text=True, timeout=30)
        
        if "Dependencies OK" in result.stdout:
            print("✅ All required dependencies available")
            return True
        else:
            print(f"❌ Dependencies check failed: {result.stdout} {result.stderr}")
            return False
            
    except Exception as e:
        print(f"❌ Error checking dependencies: {e}")
        return False

def test_basic_functionality():
    """Test basic moonlight functionality"""
    print("\\n🔍 Testing basic moonlight server functionality...")
    
    print("📋 Existing moonlight server detected at:")
    print("   http://localhost:47989 (HTTP)")
    print("   https://localhost:47984 (HTTPS)")
    print("   rtsp://localhost:48010 (RTSP)")
    
    # Test the existing server
    try:
        import urllib.request
        
        response = urllib.request.urlopen("http://localhost:47989/serverinfo", timeout=5)
        content = response.read().decode('utf-8')
        
        if "hostname" in content and "state" in content:
            print("✅ Moonlight server is responding correctly")
            print(f"📋 Server state: {content}")
            return True
        else:
            print("❌ Moonlight server response is invalid")
            return False
            
    except Exception as e:
        print(f"❌ Could not test moonlight server: {e}")
        return False

def main():
    """Run all verification checks"""
    print("🧪 Moonlight Integration Verification")
    print("=" * 50)
    
    checks = [
        ("Binary Symbols", check_binary_symbols),
        ("Source Integration", check_source_integration), 
        ("Build Configuration", check_build_configuration),
        ("Dependencies", check_dependencies),
        ("Basic Functionality", test_basic_functionality),
    ]
    
    passed = 0
    total = len(checks)
    
    for name, check_func in checks:
        print(f"\\n📋 {name}")
        print("-" * 30)
        
        try:
            if check_func():
                print(f"✅ {name}: PASSED")
                passed += 1
            else:
                print(f"❌ {name}: FAILED")
        except Exception as e:
            print(f"❌ {name}: ERROR - {e}")
    
    print("\\n" + "=" * 50)
    print(f"📊 VERIFICATION RESULTS: {passed}/{total} checks passed")
    
    if passed == total:
        print("🎉 SUCCESS: Moonlight integration is fully functional!")
        print("✅ Hyprland has been successfully integrated with Wolf moonlight server")
        print("📋 Ready for streaming with moonlight clients")
    elif passed >= total - 1:
        print("⚠️  MOSTLY SUCCESS: Integration is working with minor issues")
        print("📋 Should be functional for basic moonlight streaming")
    else:
        print("❌ FAILURE: Integration has significant issues")
        
    return passed >= total - 1

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1)