project('WindowMaker', 'c',
  version: '0.96.0',
  license: 'GPL-2.0-or-later',
  default_options: [
    'c_std=gnu11',
    'warning_level=1',
    'buildtype=debugoptimized',
  ],
  meson_version: '>= 0.54.0'
)

cc = meson.get_compiler('c')

# Global compiler arguments
add_project_arguments([
  '-DHAVE_CONFIG_H',
  '-D_GNU_SOURCE',
], language: 'c')

# Core dependencies
m_dep = cc.find_library('m', required: false)
intl_dep = dependency('intl', required: false)

x11_dep = dependency('x11', required: true)
xext_dep = dependency('xext', required: true)
xft_dep = dependency('xft', required: false)
xinerama_dep = dependency('xinerama', required: false)
xrandr_dep = dependency('xrandr', required: false)
xmu_dep = dependency('xmu', required: false)
xpm_dep = dependency('xpm', required: false)
xrender_dep = dependency('xrender', required: false)
xres_dep = dependency('xres', required: false)
xkbfile_dep = dependency('xkbfile', required: false)

fontconfig_dep = dependency('fontconfig', required: false)
freetype_dep = dependency('freetype2', required: false)

# Image dependencies for wrlib
png_dep = dependency('libpng', required: false)
jpeg_dep = dependency('libjpeg', required: false)
tiff_dep = dependency('libtiff-4', required: false)
gif_dep = cc.find_library('gif', required: false)
webp_dep = dependency('libwebp', required: false)
jxl_dep = dependency('libjxl', required: false)
cairo_dep = dependency('cairo', required: false)

# Configure Header
cdata = configuration_data()

cdata.set_quoted('PACKAGE', meson.project_name())
cdata.set_quoted('PACKAGE_NAME', meson.project_name())
cdata.set_quoted('PACKAGE_TARNAME', 'WindowMaker')
cdata.set_quoted('PACKAGE_VERSION', meson.project_version())
cdata.set_quoted('PACKAGE_STRING', meson.project_name() + ' ' + meson.project_version())
cdata.set_quoted('PACKAGE_BUGREPORT', 'wmaker-dev@googlegroups.com')
cdata.set_quoted('VERSION', meson.project_version())

cdata.set_quoted('DEF_CONFIG_PATHS', get_option('prefix') / get_option('sysconfdir') / 'WindowMaker:' + get_option('prefix') / get_option('datadir') / 'WindowMaker')
cdata.set_quoted('SYSCONFDIR', get_option('prefix') / get_option('sysconfdir'))
cdata.set_quoted('PKGDATADIR', get_option('prefix') / get_option('datadir') / 'WindowMaker')
cdata.set_quoted('PKGCONFDIR', get_option('prefix') / get_option('sysconfdir') / 'WindowMaker')
cdata.set_quoted('PIXMAPDIR', '/usr/share/pixmaps')
cdata.set_quoted('GSUSER_SUBDIR', 'GNUstep')
cdata.set_quoted('USERDATA_SUBDIR', 'Library/WindowMaker')
cdata.set_quoted('DEFAULTS_SUBDIR', 'Defaults')
cdata.set('WM_PI', '(3.14159265358979323846)')

# Header & Feature checks
headers = [
  'sys/inotify.h',
  'sys/select.h',
  'sys/time.h',
  'sys/types.h',
  'sys/wait.h',
  'sys/stat.h',
  'unistd.h',
  'string.h',
  'stdlib.h',
  'stdint.h',
  'stdnoreturn.h',
  'libintl.h',
  'poll.h',
]

foreach h : headers
  if cc.has_header(h)
    cdata.set('HAVE_' + h.underscorify().to_upper(), 1)
  endif
endforeach

if cdata.has('HAVE_SYS_INOTIFY_H')
  cdata.set('HAVE_INOTIFY', 1)
endif

if cdata.has('HAVE_POLL_H')
  cdata.set('HAVE_POLL', 1)
endif

if cdata.has('HAVE_STDNORETURN_H')
  cdata.set('HAVE_STDNORETURN', 1)
endif

cdata.set('HAVE_SETLOCALE', 1)
cdata.set('ENABLE_NLS', 1)
cdata.set('RETSIGTYPE', 'void')
cdata.set('HAVE_XCONVERTCASE', 1)
cdata.set('USE_XSHM', 1)
cdata.set('USE_MWM_HINTS', 1)
cdata.set('USE_DOCK_XDND', 1)
cdata.set('USE_NESTED_FUNC', 1)
cdata.set('USE_ICCCM_WMREPLACE', 1)

if xext_dep.found()
  cdata.set('HAVE_LIBXEXT', 1)
  cdata.set('USE_XSHAPE', 1)
endif

if xrandr_dep.found()
  cdata.set('USE_XRANDR', 1)
  cdata.set('HAVE_XRANDR', 1)
endif

if xinerama_dep.found()
  cdata.set('USE_XINERAMA', 1)
  cdata.set('HAVE_XINERAMA', 1)
endif

if xres_dep.found()
  cdata.set('USE_XRES', 1)
  cdata.set('HAVE_XRES', 1)
endif

if xkbfile_dep.found()
  cdata.set('USE_XKB', 1)
  cdata.set('HAVE_XKB', 1)
endif

if xft_dep.found()
  cdata.set('HAVE_LIBXFT', 1)
endif

if xmu_dep.found()
  cdata.set('HAVE_LIBXMU', 1)
endif

if xrender_dep.found()
  cdata.set('HAVE_LIBXRENDER', 1)
endif

if png_dep.found()
  cdata.set('USE_PNG', 1)
endif

if jpeg_dep.found()
  cdata.set('USE_JPEG', 1)
endif

if tiff_dep.found()
  cdata.set('USE_TIFF', 1)
endif

if gif_dep.found()
  cdata.set('USE_GIF', 1)
endif

if webp_dep.found()
  cdata.set('USE_WEBP', 1)
endif

if jxl_dep.found()
  cdata.set('USE_JXL', 1)
endif

if xpm_dep.found()
  cdata.set('USE_XPM', 1)
endif

configure_file(
  output: 'config.h',
  configuration: cdata
)

inc_root = include_directories('.')

# Subdirectories
subdir('wrlib')
subdir('WINGs')
subdir('wmlib')
subdir('src')
subdir('WPrefs.app')
subdir('util')

summary({
  'prefix': get_option('prefix'),
  'bindir': get_option('prefix') / get_option('bindir'),
  'sysconfdir': get_option('prefix') / get_option('sysconfdir'),
  'datadir': get_option('prefix') / get_option('datadir'),
  'XRandR': xrandr_dep.found(),
  'Xinerama': xinerama_dep.found(),
  'XRes': xres_dep.found(),
  'JPEG-XL (JXL)': jxl_dep.found(),
  'WebP': webp_dep.found(),
  'Cairo (WINGs)': cairo_dep.found(),
}, section: 'Configuration')
