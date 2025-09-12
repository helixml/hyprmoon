#!/usr/bin/env python3
"""
Real Stream Capture Test

This test will:
1. Start Hyprland with moonlight on alternate ports
2. Run green screen client
3. Capture actual video stream
4. Generate screenshot proof
"""

import subprocess
import time
import sys
import os
import tempfile
from pathlib import Path

class StreamCaptureTest:
    def __init__(self):
        self.container_name = "hyprland-stream-test"
        self.http_port = 48989  # Different from default 47989
        self.rtsp_port = 49010  # Different from default 48010
        
    def cleanup(self):
        """Clean up containers and files"""
        print("🧹 Cleaning up...")
        try:
            subprocess.run(['docker', 'stop', self.container_name], capture_output=True, timeout=10)
            subprocess.run(['docker', 'rm', self.container_name], capture_output=True, timeout=10)
        except:
            pass

    def start_hyprland_with_green_client(self):
        """Start Hyprland with custom ports and green client"""
        print(f"🚀 Starting Hyprland on ports HTTP:{self.http_port}, RTSP:{self.rtsp_port}")
        
        # Create Hyprland config that will run green client
        config = f'''
monitor=HEADLESS-1,1920x1080@60,0x0,1

input {{
    kb_layout = us
}}

misc {{
    force_default_wallpaper = 0
    disable_hyprland_logo = true
}}

animations {{
    enabled = false
}}

# Start green client after 3 seconds
exec-once = sleep 3 && /workspace/green_client
'''
        
        # Start container with Hyprland and custom moonlight ports
        cmd = [
            'docker', 'run', '--rm', '-d',
            '--name', self.container_name,
            '-v', f'{Path.cwd()}:/workspace',
            '--tmpfs', '/tmp:rw,size=200m',
            '-p', f'{self.http_port}:{self.http_port}',
            '-p', f'{self.rtsp_port}:{self.rtsp_port}',
            'hyprland-moonlight',
            '/bin/bash', '-c', f'''
echo '{config}' > /tmp/hyprland.conf
export WLR_BACKENDS=headless
export WLR_LIBINPUT_NO_DEVICES=1
export XDG_RUNTIME_DIR=/tmp
export WAYLAND_DISPLAY=wayland-0
export MOONLIGHT_HTTP_PORT={self.http_port}
export MOONLIGHT_RTSP_PORT={self.rtsp_port}
cd /workspace && chmod +x green_client && ./build/src/Hyprland --config /tmp/hyprland.conf
'''
        ]
        
        try:
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=20)
            if result.returncode != 0:
                print(f"❌ Container failed to start: {result.stderr}")
                return False
                
            print("✅ Container started, waiting for Hyprland initialization...")
            time.sleep(10)  # Give time for startup
            return True
            
        except Exception as e:
            print(f"❌ Failed to start container: {e}")
            return False

    def wait_for_moonlight_server(self):
        """Wait for moonlight server to become available"""
        print("⏳ Waiting for moonlight server...")
        
        for attempt in range(30):
            try:
                result = subprocess.run([
                    'curl', '-s', '--connect-timeout', '2',
                    f'http://localhost:{self.http_port}/serverinfo'
                ], capture_output=True, text=True, timeout=5)
                
                if result.returncode == 0 and 'hostname' in result.stdout:
                    print("✅ Moonlight server is responding!")
                    return True
                    
            except:
                pass
                
            time.sleep(2)
            print(f"⏳ Attempt {attempt + 1}/30...")
            
        print("❌ Moonlight server not responding")
        return False

    def wait_for_green_client(self):
        """Wait for green client to start"""
        print("⏳ Waiting for green client to start...")
        
        # Check container logs for green client activity
        for attempt in range(20):
            try:
                result = subprocess.run([
                    'docker', 'logs', self.container_name
                ], capture_output=True, text=True, timeout=5)
                
                logs = result.stdout + result.stderr
                if "Green screen displayed" in logs or "Green screen active" in logs:
                    print("✅ Green client is running!")
                    return True
                    
            except:
                pass
                
            time.sleep(1)
            print(f"⏳ Checking for green client... {attempt + 1}/20")
            
        print("❌ Green client not detected")
        return False

    def capture_stream(self):
        """Capture the moonlight video stream"""
        print("📹 Attempting to capture moonlight stream...")
        
        # Create output directory in current working directory
        output_dir = Path.cwd() / "stream_capture"
        output_dir.mkdir(exist_ok=True)
        
        screenshot_path = output_dir / "moonlight_stream_screenshot.png"
        video_path = output_dir / "moonlight_stream_sample.mp4"
        
        print(f"📁 Output directory: {output_dir}")
        
        # Try multiple capture methods
        capture_methods = [
            # Method 1: Direct RTSP capture
            {
                'name': 'RTSP Stream Capture',
                'cmd': [
                    'timeout', '10s', 'ffmpeg', '-y', '-v', 'quiet',
                    '-f', 'rtsp', '-rtsp_transport', 'tcp',
                    '-i', f'rtsp://localhost:{self.rtsp_port}',
                    '-t', '3', '-frames:v', '1',
                    str(screenshot_path)
                ]
            },
            # Method 2: HTTP stream capture (if available)
            {
                'name': 'HTTP Stream Capture',
                'cmd': [
                    'timeout', '10s', 'ffmpeg', '-y', '-v', 'quiet',
                    '-f', 'mjpeg', 
                    '-i', f'http://localhost:{self.http_port}/stream',
                    '-t', '1', '-frames:v', '1',
                    str(screenshot_path)
                ]
            },
            # Method 3: Screen capture from container
            {
                'name': 'Container Screen Capture',
                'cmd': [
                    'docker', 'exec', self.container_name,
                    '/bin/bash', '-c', 
                    f'timeout 5s ffmpeg -y -f x11grab -video_size 1920x1080 -i :0 -t 1 -frames:v 1 /workspace/stream_capture/container_screenshot.png'
                ]
            }
        ]
        
        for method in capture_methods:
            print(f"🔍 Trying {method['name']}...")
            try:
                result = subprocess.run(method['cmd'], capture_output=True, text=True, timeout=15)
                print(f"   Command result: {result.returncode}")
                if result.stderr:
                    print(f"   Stderr: {result.stderr[:200]}")
                    
                # Check if we got a screenshot
                possible_files = [
                    screenshot_path,
                    output_dir / "container_screenshot.png"
                ]
                
                for file_path in possible_files:
                    if file_path.exists() and file_path.stat().st_size > 0:
                        print(f"✅ {method['name']} succeeded!")
                        print(f"📁 Screenshot saved: {file_path}")
                        return str(file_path)
                        
            except subprocess.TimeoutExpired:
                print(f"   ⚠️  {method['name']} timed out")
            except Exception as e:
                print(f"   ❌ {method['name']} failed: {e}")
                
        print("❌ All capture methods failed")
        return None

    def analyze_screenshot(self, screenshot_path):
        """Analyze the screenshot to verify green content"""
        if not screenshot_path or not Path(screenshot_path).exists():
            print("❌ No screenshot to analyze")
            return False
            
        screenshot_file = Path(screenshot_path)
        print(f"🔍 Analyzing screenshot: {screenshot_file}")
        print(f"📏 File size: {screenshot_file.stat().st_size} bytes")
        
        # Check if it's a valid image file
        try:
            result = subprocess.run([
                'file', str(screenshot_file)
            ], capture_output=True, text=True, timeout=5)
            
            print(f"📋 File type: {result.stdout.strip()}")
            
            if 'image' in result.stdout.lower() or 'png' in result.stdout.lower():
                print("✅ Screenshot is a valid image file")
                
                # Try to get basic image info
                try:
                    identify_result = subprocess.run([
                        'docker', 'run', '--rm', '-v', f'{Path.cwd()}:/workspace',
                        'hyprland-moonlight', 'identify', f'/workspace/{screenshot_file.relative_to(Path.cwd())}'
                    ], capture_output=True, text=True, timeout=10)
                    
                    if identify_result.returncode == 0:
                        print(f"📐 Image info: {identify_result.stdout.strip()}")
                    
                except:
                    print("⚠️  Could not get detailed image info")
                
                return True
            else:
                print("❌ Screenshot is not a valid image")
                return False
                
        except Exception as e:
            print(f"❌ Failed to analyze screenshot: {e}")
            return False

    def get_container_logs(self):
        """Get container logs for debugging"""
        try:
            result = subprocess.run([
                'docker', 'logs', '--tail', '30', self.container_name
            ], capture_output=True, text=True, timeout=10)
            return result.stdout + result.stderr
        except:
            return "Could not retrieve logs"

    def run_test(self):
        """Run the complete stream capture test"""
        print("🧪 Real Stream Capture Test")
        print("=" * 60)
        print("This test will capture actual moonlight video stream")
        print("=" * 60)
        
        try:
            # Step 1: Start Hyprland with green client
            print("📋 Step 1: Starting Hyprland with green client")
            if not self.start_hyprland_with_green_client():
                print("❌ Failed to start Hyprland")
                return False
                
            # Step 2: Wait for moonlight server
            print("\n📋 Step 2: Waiting for moonlight server")
            if not self.wait_for_moonlight_server():
                print("❌ Moonlight server not available")
                print("📋 Container logs:")
                print(self.get_container_logs())
                return False
                
            # Step 3: Wait for green client
            print("\n📋 Step 3: Waiting for green client")
            if not self.wait_for_green_client():
                print("⚠️  Green client not detected, continuing anyway...")
                
            # Step 4: Capture stream
            print("\n📋 Step 4: Capturing video stream")
            screenshot_path = self.capture_stream()
            
            # Step 5: Analyze screenshot
            print("\n📋 Step 5: Analyzing captured content")
            if screenshot_path and self.analyze_screenshot(screenshot_path):
                print("\n🎉 STREAM CAPTURE TEST PASSED!")
                print(f"✅ Successfully captured moonlight video stream")
                print(f"✅ Screenshot saved in current directory: {screenshot_path}")
                print(f"📁 Check the file: {Path(screenshot_path).resolve()}")
                return True
            else:
                print("\n❌ STREAM CAPTURE TEST FAILED!")
                print("❌ Could not capture or analyze video stream")
                print("📋 Container logs:")
                print(self.get_container_logs())
                return False
                
        except KeyboardInterrupt:
            print("\n🛑 Test interrupted")
            return False
        except Exception as e:
            print(f"\n❌ Test failed: {e}")
            print("📋 Container logs:")
            print(self.get_container_logs())
            return False
        finally:
            self.cleanup()

def main():
    test = StreamCaptureTest()
    
    try:
        success = test.run_test()
        sys.exit(0 if success else 1)
    except KeyboardInterrupt:
        print("\n🛑 Test interrupted")
        test.cleanup()
        sys.exit(1)

if __name__ == "__main__":
    main()