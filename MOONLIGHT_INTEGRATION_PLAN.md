# Moonlight Integration Plan for Hyprland

## Overview
Integrate Wolf's moonlight streaming server directly into Hyprland compositor to enable remote streaming with hardware acceleration, full input support, and web-based pairing.

## Goals
- Native moonlight server built into Hyprland
- Hardware-accelerated video streaming (NVENC/QSV/VA-API/x264)
- Full input support (mouse, keyboard, gamepad, touch)
- Web-based PIN entry for pairing
- Zero-copy frame capture from Hyprland's renderer
- Compatible with all Moonlight clients

## Architecture

### Integration Strategy
1. **Copy Wolf's core files** into `/src/moonlight/`
2. **Replace Wolf's compositor** with Hyprland frame source
3. **Add CMoonlightManager** to Hyprland's manager system
4. **Hook into Hyprland's render pipeline** for frame capture
5. **Add build dependencies** via meson

### Directory Structure
```
/src/moonlight/
├── protocol/           # Wolf's moonlight protocol implementation
├── crypto/            # X.509 certificates, AES encryption
├── streaming/         # GStreamer pipelines and encoding
├── control/           # Input handling via ENet UDP
├── gst-plugin/        # Custom GStreamer elements
├── state/             # Configuration and session management
├── platforms/         # Hardware detection and platform abstraction
├── core/              # Essential utilities (uinput, logging, etc.)
├── rest/              # HTTP/HTTPS servers for pairing
└── managers/          # Hyprland integration layer
```

## Implementation Plan

### Phase 1: Core File Migration (30 minutes)
1. Create moonlight directory structure
2. Copy essential Wolf files
3. Remove Docker/container dependencies
4. Update include paths

### Phase 2: Build System Integration (20 minutes)
1. Add dependencies to meson.build
2. Create moonlight meson.build
3. Update main src/meson.build
4. Test compilation

### Phase 3: Hyprland Integration Layer (45 minutes)
1. Create CMoonlightManager
2. Hook into CHyprRenderer
3. Create HyprlandFrameSource GStreamer element
4. Add configuration support

### Phase 4: Frame Bridge Implementation (30 minutes)
1. Convert Aquamarine::IBuffer to GstBuffer
2. Replace Wolf's waylanddisplaysrc
3. Implement zero-copy DMA-BUF sharing
4. Handle multiple monitors

### Phase 5: Testing and Validation (15 minutes)
1. Build and test compilation
2. Test moonlight server startup
3. Verify pairing functionality
4. Test streaming with client

## File Copying Matrix

### Essential Protocol Files
- `/moonlight-protocol/moonlight.cpp` → `/src/moonlight/protocol/`
- `/moonlight-protocol/moonlight/*.hpp` → `/src/moonlight/protocol/`
- `/moonlight-protocol/crypto/` → `/src/moonlight/crypto/`
- `/moonlight-protocol/rtsp/` → `/src/moonlight/protocol/rtsp/`

### Streaming Engine
- `/moonlight-server/streaming/` → `/src/moonlight/streaming/`
- `/moonlight-server/gst-plugin/` → `/src/moonlight/gst-plugin/`
- `/moonlight-server/control/` → `/src/moonlight/control/`
- `/moonlight-server/rtp/` → `/src/moonlight/streaming/rtp/`

### Support Systems
- `/moonlight-server/state/` → `/src/moonlight/state/`
- `/moonlight-server/platforms/hw_linux.cpp` → `/src/moonlight/platforms/`
- `/moonlight-server/platforms/input_linux.cpp` → `/src/moonlight/platforms/`
- `/moonlight-server/rest/` → `/src/moonlight/rest/`
- `/core/src/platforms/linux/uinput/` → `/src/moonlight/core/uinput/`

### Configuration and Utilities
- Essential utilities from `/core/src/core/`
- Logging and helper functions
- Event system components

## Dependencies to Add

### Required System Libraries
- boost (system, thread, locale)
- gstreamer-1.0
- gstreamer-app-1.0
- libenet
- fmt
- openssl
- icu

### Vendored Dependencies (copy source)
- immer (immutable data structures)
- nanors (Reed-Solomon FEC)
- range-v3 (algorithms)
- peglib (parsing)
- tomlplusplus (configuration)
- eventbus (inter-component communication)

## Integration Points

### CMoonlightManager Interface
```cpp
class CMoonlightManager {
public:
    void init();
    void destroy();
    void startStreaming(PHLMONITOR monitor);
    void stopStreaming();
    bool isStreaming() const;
    
private:
    void onFrameReady(PHLMONITOR monitor, SP<Aquamarine::IBuffer> buffer);
    void setupGStreamerPipeline();
    void handlePairingRequest();
    
    std::unique_ptr<MoonlightState> m_state;
    GstElement* m_pipeline = nullptr;
    bool m_streaming = false;
};
```

### Render Hook Integration
```cpp
// In CHyprRenderer::endRender()
if (g_pMoonlightManager && g_pMoonlightManager->isStreaming()) {
    auto buffer = monitor->m_output->state->state().buffer;
    g_pMoonlightManager->onFrameReady(monitor, buffer);
}
```

### Configuration Integration
```ini
# hyprland.conf
moonlight {
    enable = true
    quality = high
    bitrate = 20000
    fps = 60
    audio = true
    hardware_encoding = auto
}
```

## Network Services

### Exposed Ports
- **47989** (HTTP) - Discovery and pairing
- **47984** (HTTPS) - Secure API
- **48010** (RTSP) - Stream negotiation
- **47999** (UDP) - Control protocol
- **48000** (UDP) - Video RTP
- **48002** (UDP) - Audio RTP
- **mDNS** - Service discovery

### Pairing Flow
1. Client discovers Hyprland via mDNS
2. Client generates PIN and sends pairing request
3. User opens `http://hyprland-ip:47989/pin`
4. User enters PIN from client screen
5. Cryptographic handshake completes
6. Client can now stream

## Success Criteria
- [x] Moonlight clients can discover Hyprland server
- [x] Web-based pairing works without display
- [x] Hardware-accelerated video streaming
- [x] Full input device support
- [x] Audio streaming functionality
- [x] Multiple client support
- [x] Zero-copy frame capture (where possible)

## Timeline
**Total Estimated Time: 2 hours 20 minutes**

This plan provides a complete integration of Wolf's moonlight server into Hyprland while maintaining all functionality and improving performance through direct compositor integration.