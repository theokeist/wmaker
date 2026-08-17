archive_dep = dependency('libarchive', required: false)

inc_util = include_directories('.', '../util')

util_c_args = [
  '-DETCDIR="' + get_option('prefix') / get_option('sysconfdir') + '"',
  '-DDATADIR="' + get_option('prefix') / get_option('datadir') / 'WindowMaker"',
]

executable('wxcopy',
  'wxcopy.c',
  include_directories: [inc_root, inc_util],
  dependencies: [x11_dep, xext_dep],
  install: true
)

executable('wxpaste',
  'wxpaste.c',
  include_directories: [inc_root, inc_util],
  dependencies: [x11_dep, xext_dep],
  install: true
)

executable('wdwrite',
  'wdwrite.c',
  include_directories: [inc_root, inc_util, inc_wings],
  dependencies: [wutil_dep, intl_dep],
  install: true
)

executable('wdread',
  'wdread.c',
  include_directories: [inc_root, inc_util, inc_wings],
  dependencies: [wutil_dep, intl_dep],
  install: true
)

executable('getstyle',
  ['getstyle.c', 'fontconv.c'],
  include_directories: [inc_root, inc_util, inc_wings],
  dependencies: [wutil_dep, intl_dep],
  install: true
)

executable('setstyle',
  ['setstyle.c', 'fontconv.c'],
  include_directories: [inc_root, inc_util, inc_wings],
  dependencies: [wutil_dep, x11_dep, xext_dep, intl_dep],
  install: true
)

executable('convertfonts',
  ['convertfonts.c', 'fontconv.c'],
  include_directories: [inc_root, inc_util, inc_wings],
  dependencies: [wutil_dep, intl_dep],
  install: true
)

executable('seticons',
  'seticons.c',
  include_directories: [inc_root, inc_util, inc_wings],
  dependencies: [wutil_dep, intl_dep],
  install: true
)

executable('geticonset',
  'geticonset.c',
  include_directories: [inc_root, inc_util, inc_wings],
  dependencies: [wutil_dep, intl_dep],
  install: true
)

executable('wmagnify',
  'wmagnify.c',
  include_directories: [inc_root, inc_util, inc_wings, inc_wrlib],
  dependencies: [wings_dep, wutil_dep, wraster_dep, x11_dep, xext_dep, intl_dep],
  install: true
)

executable('wmsetbg',
  'wmsetbg.c',
  c_args: util_c_args,
  include_directories: [inc_root, inc_util, inc_wings, inc_wrlib],
  dependencies: [wings_dep, wutil_dep, wraster_dep, x11_dep, xext_dep, xinerama_dep, intl_dep],
  install: true
)

executable('wmgenmenu',
  'wmgenmenu.c',
  c_args: util_c_args,
  include_directories: [inc_root, inc_util, inc_wings, inc_wrlib],
  dependencies: [wutil_dep, intl_dep],
  install: true
)

executable('wmmenugen',
  ['wmmenugen.c', 'wmmenugen_misc.c', 'wmmenugen_parse_wmconfig.c', 'wmmenugen_parse_xdg.c'],
  c_args: util_c_args,
  include_directories: [inc_root, inc_util, inc_wings, inc_wrlib],
  dependencies: [wutil_dep, intl_dep],
  install: true
)

wmiv_deps = [wings_dep, wutil_dep, wraster_dep, x11_dep, xext_dep, intl_dep]
if archive_dep.found()
  wmiv_deps += archive_dep
endif

executable('wmiv',
  ['wmiv.c', 'xdnd.c'],
  c_args: util_c_args,
  include_directories: [inc_root, inc_util, inc_wings, inc_wrlib],
  dependencies: wmiv_deps,
  install: true
)
