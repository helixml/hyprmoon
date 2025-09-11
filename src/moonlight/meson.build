# Moonlight dependencies
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

# Add dependencies to main project
project_deps += [
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
]

# Add include directories
project_inc += include_directories(
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
)

# Copy vendored dependencies (header-only libraries)
# These would normally be handled by FetchContent in CMake
# For now, we'll assume they're available system-wide or vendored separately

message('Moonlight integration enabled')