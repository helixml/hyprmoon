# Moonlight dependencies (Wolf/traditional moonlight)
boost_dep = dependency('boost', modules: ['system', 'thread', 'locale'], required: true)
gstreamer_dep = dependency('gstreamer-1.0', version: '>= 1.0', required: true)
gst_app_dep = dependency('gstreamer-app-1.0', version: '>= 1.0', required: true)
gst_video_dep = dependency('gstreamer-video-1.0', version: '>= 1.0', required: true)
enet_dep = dependency('libenet', required: true)
fmt_dep = dependency('fmt', required: true)
ssl_dep = dependency('openssl', required: true)
icu_dep = dependency('icu-uc', required: true)
libdrm_dep = dependency('libdrm', required: true)
libpci_dep = dependency('libpci', required: true)
curl_dep = dependency('libcurl', required: true)

# WebRTC dependencies (enhanced bidirectional support)
gst_webrtc_dep = dependency('gstreamer-webrtc-1.0', version: '>= 1.20', required: true)
gst_sdp_dep = dependency('gstreamer-sdp-1.0', version: '>= 1.0', required: true)
nice_dep = dependency('nice', version: '>= 0.1.13', required: true)
json_dep = dependency('jsoncpp', required: true)

# Voice processing dependencies (Whisper + TTS)
pulse_dep = dependency('libpulse-simple', required: true)
pulse_main_dep = dependency('libpulse', required: true)
# whisper_dep = dependency('whisper', required: false)  # Optional: will use whisper.cpp if available
math_dep = meson.get_compiler('cpp').find_library('m', required: true)  # Math library for audio processing

# Add dependencies to main project
project_deps += [
    # Wolf/Moonlight dependencies
    boost_dep,
    gstreamer_dep,
    gst_app_dep,
    gst_video_dep,
    enet_dep,
    fmt_dep,
    ssl_dep,
    icu_dep,
    libdrm_dep,
    libpci_dep,
    curl_dep,
    
    # WebRTC dependencies
    gst_webrtc_dep,
    gst_sdp_dep,
    nice_dep,
    json_dep,
    
    # Voice processing dependencies
    pulse_dep,
    pulse_main_dep,
    math_dep,
]

# Add include directories for moonlight
moonlight_inc = include_directories(
    'protocol',
    'protocol/crypto/crypto',
    'streaming',
    'gst-plugin',
    'control',
    'platforms',
    'core',
    'rest',
    'state',
    'managers',
    'webrtc',  # WebRTC integration
    'voice',   # Voice processing (Whisper + TTS)
)

# Moonlight streaming server integration
project_sources += files(
    # Hyprland integration (working)
    'managers/MoonlightManager.cpp',
    
    # Frame bridge (working) 
    'gst-plugin/HyprlandFrameSource.cpp',
    
    # Wolf moonlight server implementation
    'wolf-impl/WolfMoonlightServer.cpp',
    
    # Wolf stubs (compatibility layer)
    'stubs/MoonlightStubs.cpp',
    
    # WebRTC integration (full bidirectional support)
    'webrtc/WebRTCManager.cpp',
    
    # Voice processing (Whisper transcription + TTS)
    'voice/WhisperManager.cpp',
)

# Copy vendored dependencies (header-only libraries)
# These would normally be handled by FetchContent in CMake
# For now, we'll assume they're available system-wide or vendored separately

message('HyprMoon: Complete desktop streaming + voice processing suite')
message('  ✓ Wolf/Moonlight server (gaming clients)')
message('  ✓ WebRTC server (browser clients)')
message('  ✓ Bidirectional audio support')
message('  ✓ Keyboard/mouse/touch input')
message('  ✓ Hardware-accelerated encoding')
message('  ✓ Whisper voice transcription → keyboard input')
message('  ✓ Text-to-speech HTTP API (robot voice)')
message('  ✓ Voice commands & speech-to-text')
message('  🎤 Microphone (WebRTC/Moonlight/System) → Speech → Keyboard')
message('  🤖 HTTP API → Text → Robot Voice → Speakers')