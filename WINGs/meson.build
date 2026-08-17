wings_conf = configuration_data()
wings_conf.set('USE_PANGO', '// ')

configure_file(
  input: 'WINGs/WINGsP.h.in',
  output: 'WINGsP.h',
  configuration: wings_conf
)

wutil_sources = files(
  'array.c',
  'bagtree.c',
  'data.c',
  'error.c',
  'findfile.c',
  'handlers.c',
  'hashtable.c',
  'memory.c',
  'menuparser.c',
  'menuparser_macros.c',
  'misc.c',
  'notification.c',
  'proplist.c',
  'string.c',
  'tree.c',
  'userdefaults.c',
  'usleep.c',
  'wapplication.c',
  'wutil.c',
)

wings_sources = files(
  'configuration.c',
  'dragcommon.c',
  'dragdestination.c',
  'dragsource.c',
  'selection.c',
  'wappresource.c',
  'wballoon.c',
  'wbox.c',
  'wbrowser.c',
  'wbutton.c',
  'wcolor.c',
  'wcolorpanel.c',
  'wcolorwell.c',
  'wevent.c',
  'wfilepanel.c',
  'wframe.c',
  'wfont.c',
  'wfontpanel.c',
  'widgets.c',
  'winputmethod.c',
  'wlabel.c',
  'wlist.c',
  'wmenuitem.c',
  'wmisc.c',
  'wpanel.c',
  'wpixmap.c',
  'wpopupbutton.c',
  'wprogressindicator.c',
  'wruler.c',
  'wscroller.c',
  'wscrollview.c',
  'wslider.c',
  'wsplitview.c',
  'wtabview.c',
  'wtext.c',
  'wtextfield.c',
  'wview.c',
  'wwindow.c',
)

inc_wings = include_directories('.', 'WINGs', '../WINGs', '../WINGs/WINGs', '../src')

libwutil = static_library('WUtil',
  wutil_sources,
  include_directories: [inc_root, inc_wings, inc_wrlib],
  dependencies: [m_dep, intl_dep],
  install: false
)

wutil_dep = declare_dependency(
  link_with: libwutil,
  include_directories: [inc_root, inc_wings, inc_wrlib],
  dependencies: [m_dep, intl_dep]
)

wings_deps = [
  wutil_dep,
  wraster_dep,
  x11_dep,
  xext_dep,
  xft_dep,
  fontconfig_dep,
  freetype_dep,
  cairo_dep,
  m_dep,
]

libwings = static_library('WINGs',
  wings_sources,
  c_args: ['-DRESOURCE_PATH="' + get_option('prefix') / get_option('datadir') / 'WINGs"'],
  include_directories: [inc_root, inc_wings, inc_wrlib],
  dependencies: wings_deps,
  install: false
)

wings_dep = declare_dependency(
  link_with: libwings,
  include_directories: [inc_root, inc_wings, inc_wrlib],
  dependencies: wings_deps
)
