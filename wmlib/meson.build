wmlib_sources = files(
  'menu.c',
  'app.c',
  'event.c',
  'command.c',
)

inc_wmlib = include_directories('.', '../wmlib')

libwmaker = static_library('WMaker',
  wmlib_sources,
  include_directories: [inc_root, inc_wmlib],
  dependencies: [x11_dep, xext_dep],
  install: false
)

wmaker_client_dep = declare_dependency(
  link_with: libwmaker,
  include_directories: [inc_root, inc_wmlib],
  dependencies: [x11_dep, xext_dep]
)
