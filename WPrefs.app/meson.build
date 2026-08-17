wprefs_sources = files(
  'main.c',
  'WPrefs.c',
  'Appearance.c',
  'Configurations.c',
  'Docks.c',
  'Expert.c',
  'Focus.c',
  'FontSimple.c',
  'HotCornerShortcuts.c',
  'Icons.c',
  'KeyboardShortcuts.c',
  'Menu.c',
  'MenuPreferences.c',
  'MouseSettings.c',
  'Paths.c',
  'Preferences.c',
  'TexturePanel.c',
  'WindowHandling.c',
  'Effects.c',
  'Workspace.c',
  'double.c',
  'editmenu.c',
  'xmodifier.c',
)

inc_wprefs = include_directories('.', '../WPrefs.app')

executable('WPrefs',
  wprefs_sources,
  c_args: [
    '-DRESOURCE_PATH="' + get_option('prefix') / get_option('datadir') / 'WPrefs"',
    '-DWMAKER_RESOURCE_PATH="' + get_option('prefix') / get_option('datadir') / 'WindowMaker"',
  ],
  include_directories: [inc_root, inc_wprefs, inc_wings, inc_wrlib, inc_src],
  dependencies: [wings_dep, wutil_dep, wraster_dep, x11_dep, xext_dep, xft_dep, xmu_dep, m_dep, intl_dep],
  install: true
)

sed = find_program('sed')

custom_target('WPrefs.desktop',
  input: 'WPrefs.desktop.in',
  output: 'WPrefs.desktop',
  command: [sed,
    '-e', 's:#wprefs_bindir#:' + get_option('prefix') / get_option('bindir') + ':',
    '-e', 's:#wprefs_datadir#:' + get_option('prefix') / get_option('datadir') / 'WPrefs:',
    '@INPUT@'],
  capture: true,
  install: true,
  install_dir: get_option('datadir') / 'applications'
)

install_subdir('tiff', install_dir: get_option('datadir') / 'WPrefs')
install_subdir('xpm', install_dir: get_option('datadir') / 'WPrefs')
install_data(['WPrefs.tiff', 'WPrefs.xpm'], install_dir: get_option('datadir') / 'WPrefs')
