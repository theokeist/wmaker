wm_datadir = get_option('datadir') / 'WindowMaker'
wm_confdir = get_option('sysconfdir') / 'WindowMaker'

# Subdirectories containing runtime resources
install_subdir('Backgrounds', install_dir: wm_datadir)
install_subdir('Compositors', install_dir: wm_datadir)
install_subdir('Defaults', install_dir: wm_datadir)
install_subdir('IconSets', install_dir: wm_datadir)
install_subdir('Icons', install_dir: wm_datadir)
install_subdir('Pixmaps', install_dir: wm_datadir)
install_subdir('Styles', install_dir: wm_datadir)
install_subdir('Themes', install_dir: wm_datadir)

# Desktop file for display managers (GDM, LightDM, SDDM)
install_data('wmaker.desktop', install_dir: get_option('datadir') / 'xsessions')

# Non-generated menus and data scripts
nongenerated_menus = [
  'menu.ca',
  'menu.cz',
  'menu.da',
  'menu.de',
  'menu.el',
  'menu.es',
  'menu.fr',
  'menu.gl',
  'menu.he',
  'menu.hr',
  'menu.it',
  'menu.ja',
  'menu.no',
  'menu.pl',
  'menu.pt',
  'menu.ru',
  'menu.se',
  'menu.sl',
  'menu.tr',
  'menu.zh_CN',
  'plmenu.da',
  'plmenu.de',
  'plmenu.fr',
  'plmenu.hr',
  'plmenu.it',
  'autostart.sh',
  'exitscript.sh',
  'background.menu',
  'background.menu.fy',
  'background.menu.nl',
  'background.menu.sr',
  'README',
  'README.themes',
]

install_data(nongenerated_menus, install_dir: wm_datadir)

# Generated menus with substitution
sed = find_program('sed')

generated_menus = [
  'appearance.menu',
  'appearance.menu.fy',
  'appearance.menu.nl',
  'appearance.menu.sr',
  'menu',
  'menu.bg',
  'menu.fi',
  'menu.fy',
  'menu.hu',
  'menu.ko',
  'menu.nl',
  'menu.ro',
  'menu.sk',
  'menu.sr',
  'menu.zh_TW',
  'plmenu',
  'plmenu.bg',
  'plmenu.es',
  'plmenu.fi',
  'plmenu.fy',
  'plmenu.ja',
  'plmenu.ko',
  'plmenu.nl',
  'plmenu.pl',
  'plmenu.ro',
  'plmenu.sk',
  'plmenu.sr',
  'plmenu.zh_CN',
  'plmenu.zh_TW',
  'wmmacros',
]

foreach m : generated_menus
  custom_target(m,
    input: m + '.in',
    output: m,
    command: [sed,
      '-e', 's:#wprefs#:' + get_option('prefix') / get_option('bindir') / 'WPrefs:',
      '-e', 's:#wmdatadir#:' + get_option('prefix') / wm_datadir + ':',
      '@INPUT@'],
    capture: true,
    install: true,
    install_dir: wm_datadir
  )
endforeach

# System-wide configuration defaults in sysconfdir / WindowMaker
install_data('Defaults/WMGLOBAL', install_dir: wm_confdir)

custom_target('WMWindowAttributes',
  input: 'Defaults/WMWindowAttributes.in',
  output: 'WMWindowAttributes',
  command: [sed,
    '-e', 's:#extension#:tiff:',
    '@INPUT@'],
  capture: true,
  install: true,
  install_dir: wm_confdir
)

custom_target('WindowMaker_default',
  input: 'Defaults/WindowMaker.in',
  output: 'WindowMaker',
  command: [sed,
    '-e', 's:#pkgdatadir#:' + get_option('prefix') / wm_datadir + ':',
    '-e', 's:#pixmapdir#:/usr/share/pixmaps:',
    '@INPUT@'],
  capture: true,
  install: true,
  install_dir: wm_confdir
)

custom_target('WMState',
  input: 'Defaults/WMState.in',
  output: 'WMState',
  command: [sed,
    '-e', 's:#wprefs#:' + get_option('prefix') / get_option('bindir') / 'WPrefs:',
    '@INPUT@'],
  capture: true,
  install: true,
  install_dir: wm_confdir
)

custom_target('WMRootMenu',
  input: 'plmenu.in',
  output: 'WMRootMenu',
  command: [sed,
    '-e', 's:#wprefs#:' + get_option('prefix') / get_option('bindir') / 'WPrefs:',
    '-e', 's:#wmdatadir#:' + get_option('prefix') / wm_datadir + ':',
    '@INPUT@'],
  capture: true,
  install: true,
  install_dir: wm_confdir
)
