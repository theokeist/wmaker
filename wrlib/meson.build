wrlib_conf = configuration_data()
wrlib_conf.set('USE_XSHM', '')

configure_file(
  input: 'wraster.h.in',
  output: 'wraster.h',
  configuration: wrlib_conf
)

wrlib_sources = files(
  'raster.c',
  'alpha_combine.c',
  'draw.c',
  'color.c',
  'load.c',
  'save.c',
  'gradient.c',
  'xpixmap.c',
  'convert.c',
  'context.c',
  'misc.c',
  'effects.c',
  'scale.c',
  'rotate.c',
  'flip.c',
  'convolve.c',
  'save_xpm.c',
  'xutil.c',
  'load_ppm.c',
)

if gif_dep.found()
  wrlib_sources += files('load_gif.c')
endif

if jpeg_dep.found()
  wrlib_sources += files('load_jpeg.c', 'save_jpeg.c')
endif

if png_dep.found()
  wrlib_sources += files('load_png.c', 'save_png.c')
endif

if tiff_dep.found()
  wrlib_sources += files('load_tiff.c')
endif

if xpm_dep.found()
  wrlib_sources += files('load_xpm.c')
else
  wrlib_sources += files('load_xpm_normalized.c')
endif

if webp_dep.found()
  wrlib_sources += files('load_webp.c')
endif

if jxl_dep.found()
  wrlib_sources += files('load_jxl.c')
endif

wrlib_deps = [
  x11_dep,
  xext_dep,
  xmu_dep,
  xpm_dep,
  png_dep,
  jpeg_dep,
  tiff_dep,
  gif_dep,
  webp_dep,
  jxl_dep,
  m_dep,
]

inc_wrlib = include_directories('.', '../wrlib')

libwraster = static_library('wraster',
  wrlib_sources,
  include_directories: [inc_root, inc_wrlib],
  dependencies: wrlib_deps,
  install: false
)

wraster_dep = declare_dependency(
  link_with: libwraster,
  include_directories: [inc_root, inc_wrlib],
  dependencies: wrlib_deps
)
