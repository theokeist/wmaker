# Glass UI and Skeuomorphic Enhancements Roadmap

This guide outlines where to modify the Window Maker stack so that themes can
introduce rounded corners, glass-like panels, and other skeuomorphic flourishes
without abandoning the project\'s lightweight goals. The checklist calls out the
relevant libraries, modules, and configuration hooks you will touch when
implementing a more tactile look inspired by classic Aqua-era desktops.

## 1. Raster foundations (wrlib)

* **Rounded masks and blurs** — extend `wrlib/scale.c` and the new easing
  helpers in `wrlib/effects.c` so they can generate alpha masks with corner
  radii, drop-shadow falloffs, and glass reflections. Feed the results through
  `RCreateBuffer`/`RRenderBuffer` before returning them as `RImage` objects so
  callers receive premultiplied pixel data ready for compositing.
* **Dynamic radius selection** — expose helpers that compute radii based on the
  widget size to avoid pixelated corners on large docks. A future
  `RMakeRoundedFrame()` wrapper can mirror the existing `RFitImageDimensions()`
  API and hide the math for menus, icons, and applets.
* **High-DPI aware sampling** — when scaling artwork, rely on
  `RScaleImageSmooth()` (added earlier for icon quality) so that larger radii and
  blur kernels stay crisp. For an even softer “glass” sheen, run the blurred
  highlight through the shared transition curves before blending.

## 2. Toolkit hooks (WINGs)

* **Clip views and panels** — update `WINGs/wbutton.c`, `WINGs/wmenu.c`, and
  other chrome painters to accept an optional rounded-rectangle mask from
  wrlib. Because WINGs already isolates drawing through `wDrawImageOnDrawable`
  and `wDrawBevel`, you can insert the mask just before the bevel stroke to clip
  corners.
* **Scrollbar polish** — revise `WINGs/wscrollview.c` so scrollbars respect
  larger thumb textures and inherit the same corner radius as the container. You
  can reuse the transition curves when animating hover/drag feedback for smoother
  scrolling.
* **Theme schema** — extend the defaults schema in `WINGs/WINGsDefaults.c` with
  keys such as `MenuCornerRadius`, `PanelShadowSize`, and `GlassHighlightOpacity`.
  WPrefs will expose these controls, while older themes will keep their current
  look because the defaults fall back to zero radius and classic bevels.

## 3. Core Window Maker integration

* **Menu renderer** — touch `src/menu.c` so each `WMenu` asks WINGs for a rounded
  mask when painting backgrounds. Combine it with the existing indicator glyphs
  (`MI_*` types) so icons and toggles align with the new padding.
* **Window frames** — enhance `src/framewin.c` by introducing per-corner radius
  support when building the client frame pixmaps. Respect the user’s
  preferences (stored via `defaults.c`) and update focus highlights to follow the
  curved edge. When windows move, `doWindowMove()` already leverages the shared
  transition curves; ensure it preserves the mask while animating so corners stay
  intact.
* **Dock tiles** — in `src/dock.c`, reuse the same rounded masking helper for
  application tiles and appicons. Because dockapps can provide arbitrary imagery,
  guard the mask application behind the new defaults so legacy square tiles keep
  working.

## 4. Preferences (WPrefs.app)

* **New controls** — build on the existing Window Handling panel by adding
  sliders for corner radius, shadow depth, and highlight intensity. Store the
  settings in the user defaults using the schema keys defined above. The icon
  preview infrastructure (`WPrefs.app/Icons.c`) already scales assets smoothly,
  so you can render a live rounded preview by calling the upcoming
  `RMakeRoundedFrame()` helper.
* **Live feedback** — trigger `WMSetLookChanged()` (see `WPrefs.app/Configurations.c`)
  whenever the radius or shadow settings change so the running window manager
  re-reads the defaults and repaints frames without a restart.

## 5. Dockapps and GNUstep alignment

* **Dockapp guidelines** — document that dockapps can opt into rounded tiles by
  querying the defaults via `wmlib` helpers (for example `WApplicationGetDockHints`) and
  drawing against the same masks provided by wrlib. Encourage authors to ship
  higher-resolution assets so scaling to larger radii stays smooth.
* **GNUstep harmony** — because GNUstep applications already support richer
  theming, keep the new defaults compatible by mirroring key names where
  possible (`GSThemeCornerRadius`, etc.). That way GNUstep panels can pick up the
  same values when running alongside Window Maker.

## 6. Visual QA checklist

After implementing the hooks above, rebuild and confirm the following scenarios:

1. **Menus** — right-click menus display rounded corners, subtle glass gloss, and
   dynamic icons without clipping text.
2. **Window frames** — focused and unfocused frames keep their curved edges,
   gradients, and transition-curve-driven animations during move/resize.
3. **Scroll views** — WINGs applications (WPrefs, WMPrefs) show smoother
   scrolling with rounded container edges and consistent highlight behavior.
4. **Dock tiles** — standard appicons and dockapps receive the rounded mask while
   legacy square themes remain unchanged when the new defaults are disabled.
5. **High-DPI** — launching on a Retina/HiDPI display produces crisp corners and
   shadows thanks to the wrlib scaling improvements.

Following this roadmap keeps the skeuomorphic glass look grounded in reusable
infrastructure: wrlib provides the image primitives, WINGs exposes them to
applications, and the core window manager consumes them for menus, frames, and
window transitions. WPrefs closes the loop by letting users dial in their
preferred amount of curvature and gloss.
