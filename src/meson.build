configure_file(
  input: 'wconfig.h.in',
  output: 'wconfig.h',
  copy: true
)

src_sources = files(
  'actions.c',
  'appicon.c',
  'application.c',
  'appmenu.c',
  'balloon.c',
  'client.c',
  'colormap.c',
  'compositor.c',
  'cycling.c',
  'defaults.c',
  'dialog.c',
  'dock.c',
  'dockedapp.c',
  'event.c',
  'framewin.c',
  'geomview.c',
  'icon.c',
  'keytree.c',
  'main.c',
  'menu.c',
  'misc.c',
  'monitor.c',
  'moveres.c',
  'pixmap.c',
  'placement.c',
  'properties.c',
  'randr.c',
  'resources.c',
  'rootmenu.c',
  'screen.c',
  'session.c',
  'shutdown.c',
  'stacking.c',
  'startup.c',
  'superfluous.c',
  'switchmenu.c',
  'switchpanel.c',
  'texture.c',
  'usermenu.c',
  'wcore.c',
  'wdefaults.c',
  'window.c',
  'winmenu.c',
  'winspector.c',
  'wmspec.c',
  'workspace.c',
  'wsmap.c',
  'xdnd.c',
  'xinerama.c',
  'xmodifier.c',
  'xutil.c',
  'motif.c',
)

if host_machine.system() == 'linux'
  src_sources += files('osdep_linux.c')
elif host_machine.system() == 'darwin'
  src_sources += files('osdep_darwin.c')
elif host_machine.system().contains('bsd')
  src_sources += files('osdep_bsd.c')
else
  src_sources += files('osdep_stub.c')
endif

inc_src = include_directories('.', '../src')

src_deps = [
  wings_dep,
  wutil_dep,
  wraster_dep,
  x11_dep,
  xext_dep,
  xft_dep,
  xinerama_dep,
  xrandr_dep,
  xres_dep,
  xkbfile_dep,
  m_dep,
  intl_dep,
]

executable('wmaker',
  src_sources,
  c_args: [
    '-DWMAKER_RESOURCE_PATH="' + get_option('prefix') / get_option('datadir') / 'WindowMaker"',
  ],
  include_directories: [inc_root, inc_src, inc_wings, inc_wrlib],
  dependencies: src_deps,
  install: true
)
