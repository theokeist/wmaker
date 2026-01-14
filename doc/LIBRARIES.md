# Window Maker Library Overview

Window Maker is organized around a small set of in-tree libraries that complement the
X11 toolkits it links against. The goal of this document is to catalog those
components, highlight what they provide, and show how they interoperate when you
build or extend the window manager.

## Core relationships

* **WINGs** supplies the lightweight, NeXTSTEP-inspired widget toolkit used by
  the window manager and auxiliary tools. The project intentionally keeps it
  small and fast, prioritizing support for Window Maker itself rather than large
  standalone applications.【F:README†L38-L56】【F:WINGs/README†L1-L39】
* **wrlib** (often referred to as the WindowMaker raster library) handles image
  decoding, transformation, and X11 visual conversions. Window Maker relies on it
  for pixmap management, scaling, and caching behavior that keeps textures and
  icons responsive.【F:wrlib/README†L1-L20】
* **wmlib** bundles shared helpers for applications that integrate with
  Window Maker—wrapping menu definitions, event processing, and client commands
  in a reusable interface for dockapps or test utilities.【F:wmlib/WMaker.h†L25-L156】

Those libraries sit alongside the window manager sources in `src/`, which glue the
pieces together with Xlib, libXext, libXrandr, libXinerama, and libXpm at build
time. Optional integrations (such as `xdnd` drag and drop support) are gated by
configure-time checks in `src/Makefile.am`.

## Library responsibilities

### WINGs

WINGs exposes an OpenStep-inspired API in C, covering windows, panels, lists,
sliders, tab views, color pickers, drag-and-drop, and configuration storage. Its
primary role is to render Window Maker’s controls and basic utilities, while
remaining light enough to keep startup time low. Because it intentionally omits
rich text, advanced layout, and other heavyweight widgets, projects that need a
full application framework are encouraged to target GNUstep instead.【F:WINGs/README†L1-L64】

### wrlib

The raster library focuses on turning arbitrary image data into X11-ready
pixmaps. It provides scaling helpers, color management, and caches tuned for the
icon sizes that Window Maker displays most often. Runtime knobs like
`RIMAGE_CACHE` and `RIMAGE_CACHE_SIZE` let administrators balance memory use and
responsiveness when working with large icon sets.【F:wrlib/README†L1-L20】 Recent
work also adds easing helpers such as `REffectProgressForCurve`, giving both the
window manager and WPrefs access to consistent animation curves alongside the
existing scaling primitives.【F:wrlib/wraster.h.in†L74-L86】【F:wrlib/effects.c†L1-L40】

### wmlib

`wmlib` implements the `WMaker` convenience API used by bundled tools in `util/`
and the `wtest` harness under `test/`. Its modules wrap client creation,
communication with the window manager, and menu scripting so that external
programs can behave like first-class Window Maker clients without duplicating
Xlib boilerplate.【F:wmlib/WMaker.h†L108-L156】

## Interacting with GNUstep

Window Maker brands itself as the “official” window manager for GNUstep, which
means its libraries deliberately complement GNUstep’s Objective-C frameworks
instead of duplicating them. WINGs focuses on the lean control set the window
manager needs, while GNUstep remains the answer for complex applications. The
result is a layered ecosystem where Window Maker handles window management and
panel chrome, WINGs covers lightweight widgets, and GNUstep powers full-featured
apps when the environment calls for them.【F:README†L38-L56】

