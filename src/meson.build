# Initialize project variables
project_sources = []
project_deps = []

# Get all C++ source files for main Hyprland (excluding moonlight)
globber = run_command('sh', '-c', 'find . -name "*.cpp" -not -path "./moonlight/*" | sort', check: true)
src = globber.stdout().strip().split('\n')
project_sources += files(src)

# Core Hyprland dependencies (wlroots version)
project_deps += [
  server_protos,
  dependency('wayland-server'),
  dependency('wayland-client'),
  wlroots,
  dependency('cairo'),
  dependency('hyprcursor'),
  dependency('hyprlang', version: '>= 0.3.2'),
  dependency('hyprutils', version: '>= 0.1.1'),
  dependency('libdrm'),
  dependency('egl'),
  dependency('xkbcommon'),
  dependency('libinput'),
  xcb_dep,
  xcb_composite_dep,
  xcb_errors_dep,
  xcb_icccm_dep,
  xcb_render_dep,
  xcb_res_dep,
  xcb_xfixes_dep,
  backtrace_dep,
  epoll_dep,
  udis86,
  dependency('pixman-1'),
  dependency('gl', 'opengl'),
  dependency('threads'),
  dependency('pango'),
  dependency('pangocairo'),
  dependency('uuid'),
]

# Include moonlight integration if enabled
moonlight_inc = []
if get_option('with_moonlight')
  subdir('moonlight')
endif

executable('Hyprland', project_sources,
  cpp_args: ['-DWLR_USE_UNSTABLE'],
  link_args: '-rdynamic',
  cpp_pch: 'pch/pch.hpp',
  dependencies: project_deps,
  include_directories: moonlight_inc,
  install : true
)