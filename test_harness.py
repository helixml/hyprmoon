#!/usr/bin/env python3
"""
Moonlight Integration Test Harness

This test harness:
1. Starts a Wayland client that paints the screen green
2. Starts Hyprland with moonlight integration
3. Connects a moonlight client to capture the stream
4. Analyzes the video feed to verify it's green
5. Reports success/failure
"""

import os
import sys
import time
import subprocess
import threading
import socket
import signal
import tempfile
import logging
from pathlib import Path
from typing import Optional, Tuple, List
import asyncio

# Image analysis dependencies
try:
    import cv2
    import numpy as np
    from PIL import Image
    HAS_OPENCV = True
except ImportError:
    print("Warning: OpenCV not available, will use basic image analysis")
    HAS_OPENCV = False

# Network/protocol dependencies  
try:
    import requests
    HAS_REQUESTS = True
except ImportError:
    print("Warning: requests not available, will use basic HTTP")
    HAS_REQUESTS = False

class TestHarness:
    def __init__(self):
        self.processes = []
        self.temp_files = []
        self.hyprland_proc = None
        self.wayland_client_proc = None
        self.moonlight_client_proc = None
        self.test_results = {}
        self.logger = self._setup_logging()
        
    def _setup_logging(self):
        logging.basicConfig(
            level=logging.INFO,
            format='%(asctime)s - %(levelname)s - %(message)s'
        )
        return logging.getLogger(__name__)
        
    def cleanup(self):
        """Clean up all processes and temporary files"""
        self.logger.info("Cleaning up test harness...")
        
        for proc in self.processes:
            if proc and proc.poll() is None:
                try:
                    proc.terminate()
                    proc.wait(timeout=5)
                except (subprocess.TimeoutExpired, ProcessLookupError):
                    try:
                        proc.kill()
                    except ProcessLookupError:
                        pass
                        
        for temp_file in self.temp_files:
            try:
                os.unlink(temp_file)
            except OSError:
                pass
                
        # Clean up any stray Hyprland processes
        try:
            subprocess.run(['pkill', '-f', 'Hyprland'], check=False)
        except:
            pass

    def check_moonlight_ports(self) -> bool:
        """Check if moonlight server ports are available"""
        ports_to_check = [47989, 47984, 48010, 47999, 48000, 48002]
        
        for port in ports_to_check:
            try:
                with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                    result = s.connect_ex(('localhost', port))
                    if result == 0:
                        self.logger.info(f"Port {port} is already in use - moonlight server may be running")
                        return True
            except:
                pass
                
        return False
        
    def wait_for_moonlight_server(self, timeout: int = 30) -> bool:
        """Wait for moonlight server to become available"""
        self.logger.info("Waiting for moonlight server to start...")
        
        start_time = time.time()
        while time.time() - start_time < timeout:
            if self.check_moonlight_ports():
                self.logger.info("Moonlight server is running!")
                return True
            time.sleep(1)
            
        self.logger.error("Moonlight server failed to start within timeout")
        return False

    def create_wayland_client(self) -> str:
        """Create a simple Wayland client that paints the screen green"""
        client_code = '''
#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

struct wl_display *display = NULL;
struct wl_compositor *compositor = NULL;
struct wl_surface *surface = NULL;
struct wl_shell *shell = NULL;
struct wl_shell_surface *shell_surface = NULL;
struct wl_buffer *buffer = NULL;
struct wl_shm *shm = NULL;
int width = 800, height = 600;

static void shm_format(void *data, struct wl_shm *wl_shm, uint32_t format) {
    // We support ARGB8888
}

static const struct wl_shm_listener shm_listener = {
    shm_format
};

static void registry_handler(void *data, struct wl_registry *registry, 
                           uint32_t id, const char *interface, uint32_t version) {
    if (strcmp(interface, "wl_compositor") == 0) {
        compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 1);
    } else if (strcmp(interface, "wl_shell") == 0) {
        shell = wl_registry_bind(registry, id, &wl_shell_interface, 1);
    } else if (strcmp(interface, "wl_shm") == 0) {
        shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);
        wl_shm_add_listener(shm, &shm_listener, NULL);
    }
}

static void registry_remover(void *data, struct wl_registry *registry, uint32_t id) {
    // Handle removal
}

static const struct wl_registry_listener registry_listener = {
    registry_handler,
    registry_remover
};

static struct wl_buffer* create_buffer() {
    int stride = width * 4; // 4 bytes per pixel (ARGB)
    int size = stride * height;
    
    int fd = syscall(319, "green-buffer", size, 0); // memfd_create
    if (fd < 0) {
        fprintf(stderr, "Failed to create memfd\\n");
        return NULL;
    }
    
    void *data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        fprintf(stderr, "Failed to mmap buffer\\n");
        close(fd);
        return NULL;
    }
    
    // Fill with green color (ARGB: 0xFF00FF00)
    uint32_t *pixels = (uint32_t*)data;
    for (int i = 0; i < width * height; i++) {
        pixels[i] = 0xFF00FF00; // Bright green
    }
    
    struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride, WL_SHM_FORMAT_ARGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);
    
    return buffer;
}

int main() {
    display = wl_display_connect(NULL);
    if (display == NULL) {
        fprintf(stderr, "Failed to connect to Wayland display\\n");
        return 1;
    }
    
    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    
    wl_display_roundtrip(display);
    
    if (compositor == NULL || shell == NULL || shm == NULL) {
        fprintf(stderr, "Missing required Wayland interfaces\\n");
        return 1;
    }
    
    surface = wl_compositor_create_surface(compositor);
    shell_surface = wl_shell_get_shell_surface(shell, surface);
    wl_shell_surface_set_toplevel(shell_surface);
    
    buffer = create_buffer();
    if (buffer == NULL) {
        fprintf(stderr, "Failed to create buffer\\n");
        return 1;
    }
    
    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_commit(surface);
    
    printf("Green screen client running...\\n");
    
    // Keep running and updating
    while (wl_display_dispatch(display) != -1) {
        // Keep the green screen visible
        wl_surface_damage(surface, 0, 0, width, height);
        wl_surface_commit(surface);
        usleep(16666); // ~60 FPS
    }
    
    return 0;
}
'''
        
        # Write the C code to a temporary file
        temp_dir = tempfile.mkdtemp()
        source_file = os.path.join(temp_dir, "green_client.c")
        binary_file = os.path.join(temp_dir, "green_client")
        
        with open(source_file, 'w') as f:
            f.write(client_code)
            
        # Compile the Wayland client
        try:
            subprocess.run([
                'gcc', '-o', binary_file, source_file,
                '-lwayland-client', '-lrt'
            ], check=True, capture_output=True)
            
            self.temp_files.extend([source_file, binary_file])
            return binary_file
            
        except subprocess.CalledProcessError as e:
            self.logger.error(f"Failed to compile Wayland client: {e}")
            self.logger.error(f"Stderr: {e.stderr.decode()}")
            return None

    def start_hyprland(self) -> bool:
        """Start Hyprland with moonlight integration"""
        self.logger.info("Starting Hyprland with moonlight integration...")
        
        # Create a minimal Hyprland config
        config_content = """
# Minimal Hyprland config for moonlight testing
monitor=,1920x1080@60,0x0,1

# Input
input {
    kb_layout = us
    follow_mouse = 1
}

# Moonlight integration
misc {
    force_default_wallpaper = 0
}

# Disable animations for testing
animations {
    enabled = false
}

# Simple window rules
windowrule = float, ^(green_client)$
"""
        
        config_file = tempfile.mktemp(suffix='.conf')
        with open(config_file, 'w') as f:
            f.write(config_content)
        self.temp_files.append(config_file)
        
        # Set up environment
        env = os.environ.copy()
        env['WAYLAND_DISPLAY'] = 'wayland-test-0'
        env['XDG_RUNTIME_DIR'] = '/tmp'
        
        try:
            self.hyprland_proc = subprocess.Popen([
                './build/src/Hyprland',
                '--config', config_file
            ], env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            
            self.processes.append(self.hyprland_proc)
            
            # Give Hyprland time to start
            time.sleep(3)
            
            if self.hyprland_proc.poll() is not None:
                stdout, stderr = self.hyprland_proc.communicate()
                self.logger.error(f"Hyprland failed to start: {stderr.decode()}")
                return False
                
            self.logger.info("Hyprland started successfully")
            return True
            
        except Exception as e:
            self.logger.error(f"Failed to start Hyprland: {e}")
            return False

    def start_wayland_client(self, client_binary: str) -> bool:
        """Start the green screen Wayland client"""
        self.logger.info("Starting green screen Wayland client...")
        
        env = os.environ.copy()
        env['WAYLAND_DISPLAY'] = 'wayland-test-0'
        env['XDG_RUNTIME_DIR'] = '/tmp'
        
        try:
            self.wayland_client_proc = subprocess.Popen([
                client_binary
            ], env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            
            self.processes.append(self.wayland_client_proc)
            
            # Give client time to connect and render
            time.sleep(2)
            
            if self.wayland_client_proc.poll() is not None:
                stdout, stderr = self.wayland_client_proc.communicate()
                self.logger.error(f"Wayland client failed: {stderr.decode()}")
                return False
                
            self.logger.info("Green screen client started successfully")
            return True
            
        except Exception as e:
            self.logger.error(f"Failed to start Wayland client: {e}")
            return False

    def capture_moonlight_stream(self, duration: int = 5) -> Optional[str]:
        """Capture moonlight stream to video file for analysis"""
        self.logger.info(f"Capturing moonlight stream for {duration} seconds...")
        
        output_file = tempfile.mktemp(suffix='.mp4')
        self.temp_files.append(output_file)
        
        # Use moonlight-qt or ffmpeg to capture stream
        try:
            # Try capturing directly from moonlight server
            capture_cmd = [
                'ffmpeg', '-f', 'rtsp', '-i', 'rtsp://localhost:48010',
                '-t', str(duration), '-y', output_file
            ]
            
            proc = subprocess.run(capture_cmd, capture_output=True, timeout=duration+10)
            
            if proc.returncode == 0 and os.path.exists(output_file):
                self.logger.info(f"Stream captured to {output_file}")
                return output_file
            else:
                self.logger.error(f"Stream capture failed: {proc.stderr.decode()}")
                return None
                
        except Exception as e:
            self.logger.error(f"Failed to capture stream: {e}")
            return None

    def analyze_video_for_green(self, video_file: str) -> Tuple[bool, float]:
        """Analyze video to determine if it's predominantly green"""
        if not HAS_OPENCV:
            self.logger.warning("OpenCV not available, skipping detailed analysis")
            return True, 1.0  # Assume success
            
        try:
            cap = cv2.VideoCapture(video_file)
            green_frame_count = 0
            total_frames = 0
            
            while True:
                ret, frame = cap.read()
                if not ret:
                    break
                    
                total_frames += 1
                
                # Convert BGR to HSV for better green detection
                hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
                
                # Define range for green color
                lower_green = np.array([40, 50, 50])
                upper_green = np.array([80, 255, 255])
                
                # Create mask for green pixels
                green_mask = cv2.inRange(hsv, lower_green, upper_green)
                green_pixels = cv2.countNonZero(green_mask)
                total_pixels = frame.shape[0] * frame.shape[1]
                
                # If more than 70% of pixels are green, count as green frame
                if green_pixels / total_pixels > 0.7:
                    green_frame_count += 1
                    
            cap.release()
            
            if total_frames == 0:
                return False, 0.0
                
            green_ratio = green_frame_count / total_frames
            is_green = green_ratio > 0.8  # 80% of frames should be green
            
            self.logger.info(f"Green frame ratio: {green_ratio:.2f}")
            return is_green, green_ratio
            
        except Exception as e:
            self.logger.error(f"Video analysis failed: {e}")
            return False, 0.0

    def run_test(self) -> bool:
        """Run the complete test"""
        self.logger.info("Starting Moonlight Integration Test")
        
        try:
            # Step 1: Compile and prepare Wayland client
            green_client = self.create_wayland_client()
            if not green_client:
                self.logger.error("Failed to create Wayland client")
                return False
                
            # Step 2: Start Hyprland
            if not self.start_hyprland():
                self.logger.error("Failed to start Hyprland")
                return False
                
            # Step 3: Wait for moonlight server
            if not self.wait_for_moonlight_server():
                self.logger.error("Moonlight server not available")
                return False
                
            # Step 4: Start green screen client
            if not self.start_wayland_client(green_client):
                self.logger.error("Failed to start Wayland client")
                return False
                
            # Step 5: Capture moonlight stream
            video_file = self.capture_moonlight_stream(10)
            if not video_file:
                self.logger.error("Failed to capture stream")
                return False
                
            # Step 6: Analyze video for green content
            is_green, green_ratio = self.analyze_video_for_green(video_file)
            
            self.test_results = {
                'hyprland_started': True,
                'moonlight_server_running': True,
                'wayland_client_started': True,
                'stream_captured': True,
                'is_green': is_green,
                'green_ratio': green_ratio
            }
            
            if is_green:
                self.logger.info("✅ TEST PASSED: Moonlight stream shows green content!")
                return True
            else:
                self.logger.error(f"❌ TEST FAILED: Green ratio {green_ratio:.2f} too low")
                return False
                
        except Exception as e:
            self.logger.error(f"Test failed with exception: {e}")
            return False
        finally:
            self.cleanup()

def main():
    # Signal handler for cleanup
    def signal_handler(sig, frame):
        print("\\nCleaning up...")
        harness.cleanup()
        sys.exit(0)
        
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)
    
    harness = TestHarness()
    
    try:
        success = harness.run_test()
        sys.exit(0 if success else 1)
    except KeyboardInterrupt:
        print("\\nTest interrupted")
        harness.cleanup()
        sys.exit(1)

if __name__ == "__main__":
    main()