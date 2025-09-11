#!/usr/bin/env python3
"""
Simple Moonlight Integration Test

This test verifies:
1. Hyprland starts with moonlight integration
2. Moonlight server ports become available  
3. Basic moonlight server responses work
"""

import subprocess
import time
import socket
import threading
import sys
import os
import signal
from pathlib import Path

class SimpleMoonlightTest:
    def __init__(self):
        self.hyprland_proc = None
        
    def check_port(self, port, timeout=5):
        """Check if a port is open"""
        start_time = time.time()
        while time.time() - start_time < timeout:
            try:
                with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                    s.settimeout(1)
                    result = s.connect_ex(('localhost', port))
                    if result == 0:
                        return True
            except:
                pass
            time.sleep(0.5)
        return False
        
    def test_http_response(self, port=47989):
        """Test basic HTTP response from moonlight server"""
        try:
            import urllib.request
            import urllib.error
            
            # Try to get serverinfo endpoint
            url = f"http://localhost:{port}/serverinfo"
            req = urllib.request.Request(url)
            
            with urllib.request.urlopen(req, timeout=5) as response:
                content = response.read().decode('utf-8')
                print(f"✅ HTTP Response from moonlight server:")
                print(f"   Status: {response.getcode()}")
                print(f"   Content-Type: {response.headers.get('Content-Type', 'unknown')}")
                if 'hostname' in content.lower() or 'hyprland' in content.lower():
                    print(f"   ✅ Response contains expected moonlight server info")
                    return True
                else:
                    print(f"   ❌ Response doesn't look like moonlight server: {content[:100]}")
                    return False
                    
        except Exception as e:
            print(f"❌ HTTP test failed: {e}")
            return False
            
    def start_hyprland(self):
        """Start Hyprland in containerized environment"""
        print("🚀 Starting Hyprland with moonlight integration...")
        
        # Create minimal config
        config_content = """
# Minimal config for moonlight testing
monitor=HEADLESS-1,1920x1080@60,0x0,1

input {
    kb_layout = us
}

misc {
    force_default_wallpaper = 0
    disable_hyprland_logo = true
}

animations {
    enabled = false
}
"""
        
        # Write config to container
        config_path = "/tmp/hyprland_test.conf"
        
        # Start Hyprland in container with proper environment
        cmd = [
            'docker', 'run', '--rm', '-d',
            '--name', 'hyprland-moonlight-test',
            '-v', f'{Path.cwd()}:/workspace',
            '--tmpfs', '/tmp:rw,noexec,nosuid,size=100m',
            '-p', '47989:47989',  # HTTP port
            '-p', '47984:47984',  # HTTPS port  
            '-p', '48010:48010',  # RTSP port
            '-p', '47999:47999',  # Control port
            'hyprland-moonlight',
            '/bin/bash', '-c', f'''
            echo '{config_content}' > {config_path}
            export WLR_BACKENDS=headless
            export WLR_LIBINPUT_NO_DEVICES=1
            export XDG_RUNTIME_DIR=/tmp
            export WAYLAND_DISPLAY=wayland-test-0
            cd /workspace && ./build/src/Hyprland --config {config_path}
            '''
        ]
        
        try:
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=10)
            if result.returncode != 0:
                print(f"❌ Failed to start Hyprland container: {result.stderr}")
                return False
                
            print("✅ Hyprland container started")
            return True
            
        except subprocess.TimeoutExpired:
            print("❌ Hyprland container start timed out")
            return False
        except Exception as e:
            print(f"❌ Failed to start Hyprland: {e}")
            return False
            
    def stop_hyprland(self):
        """Stop Hyprland container"""
        try:
            subprocess.run(['docker', 'stop', 'hyprland-moonlight-test'], 
                         capture_output=True, timeout=10)
            print("✅ Hyprland container stopped")
        except:
            pass
            
    def get_container_logs(self):
        """Get container logs for debugging"""
        try:
            result = subprocess.run(['docker', 'logs', 'hyprland-moonlight-test'], 
                                  capture_output=True, text=True, timeout=5)
            return result.stdout + result.stderr
        except:
            return "Could not retrieve logs"
            
    def run_test(self):
        """Run the complete test"""
        print("🧪 Starting Simple Moonlight Integration Test")
        print("=" * 50)
        
        try:
            # Step 1: Start Hyprland with moonlight
            if not self.start_hyprland():
                print("❌ TEST FAILED: Could not start Hyprland")
                return False
                
            # Step 2: Wait for moonlight server to start
            print("⏳ Waiting for moonlight server to start...")
            time.sleep(5)  # Give it time to initialize
            
            # Step 3: Check if moonlight HTTP port is available
            print("🔍 Checking moonlight HTTP port (47989)...")
            if not self.check_port(47989, timeout=15):
                print("❌ Moonlight HTTP port not available")
                print("📋 Container logs:")
                print(self.get_container_logs())
                return False
            print("✅ Moonlight HTTP port is available")
            
            # Step 4: Test HTTP response
            print("🌐 Testing moonlight server HTTP response...")
            if not self.test_http_response():
                print("❌ Moonlight server HTTP test failed")
                return False
                
            # Step 5: Check other ports
            ports_to_check = [
                (47984, "HTTPS"),
                (48010, "RTSP"), 
                (47999, "Control")
            ]
            
            for port, name in ports_to_check:
                print(f"🔍 Checking {name} port ({port})...")
                if self.check_port(port, timeout=5):
                    print(f"✅ {name} port is available")
                else:
                    print(f"⚠️  {name} port not available (may be normal)")
                    
            print("=" * 50)
            print("🎉 TEST PASSED: Moonlight integration is working!")
            print("✅ Hyprland started successfully")
            print("✅ Moonlight server is running") 
            print("✅ HTTP endpoints are responding")
            print("📋 Next steps:")
            print("   - Connect a moonlight client to test streaming")
            print("   - Use the web interface at http://localhost:47989/pin/")
            return True
            
        except KeyboardInterrupt:
            print("\\n🛑 Test interrupted by user")
            return False
        except Exception as e:
            print(f"❌ TEST FAILED: {e}")
            print("📋 Container logs:")
            print(self.get_container_logs())
            return False
        finally:
            self.stop_hyprland()

def main():
    def signal_handler(sig, frame):
        print("\\n🛑 Cleaning up...")
        test.stop_hyprland()
        sys.exit(0)
        
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)
    
    test = SimpleMoonlightTest()
    
    try:
        success = test.run_test()
        sys.exit(0 if success else 1)
    except KeyboardInterrupt:
        print("\\n🛑 Test interrupted")
        test.stop_hyprland()
        sys.exit(1)

if __name__ == "__main__":
    main()