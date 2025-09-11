# Initialize project variables
project_sources = []
project_deps = []
project_inc = []

# Get all C++ source files for main Hyprland (excluding moonlight)
globber = run_command('sh', '-c', 'find . -name "*.cpp" -not -path "./moonlight/*" | sort', check: true)
src = globber.stdout().strip().split('\n')
project_sources += files(src)

# Core Hyprland dependencies
project_deps += [
  server_protos,
  aquamarine,
  hyprcursor,
  hyprgraphics,
  hyprlang,
  hyprutils,
  dependency('gbm'),
  dependency('xcursor'),
  dependency('wayland-server'),
  dependency('wayland-client'),
  dependency('cairo'),
  dependency('libdrm'),
  dependency('egl'),
  dependency('xkbcommon'),
  dependency('libinput', version: '>=1.28'),
  dependency('re2'),
  xcb_dep,
  xcb_composite_dep,
  xcb_errors_dep,
  xcb_icccm_dep,
  xcb_render_dep,
  xcb_res_dep,
  xcb_xfixes_dep,
  backtrace_dep,
  epoll_dep,
  inotify_dep,
  gio_dep,
  tracy,
  dependency('udis86'),
  dependency('pixman-1'),
  dependency('gl', 'opengl'),
  dependency('threads'),
  dependency('pango'),
  dependency('pangocairo'),
  dependency('uuid'),
]

# Include moonlight integration if enabled
if get_option('with_moonlight')
  subdir('moonlight')
endif

executable(
  'Hyprland',
  project_sources,
  link_args: '-rdynamic',
  cpp_pch: 'pch/pch.hpp',
  dependencies: project_deps,
  include_directories: project_inc,
  install: true,
)

install_symlink(
  'hyprland',
  install_dir: get_option('bindir'),
  pointing_to: 'Hyprland',
)