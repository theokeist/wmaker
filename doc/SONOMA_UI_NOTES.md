# Sonoma UI Implementation Blueprint

This guide captures the architectural work required to deliver a macOS Sonoma-
style experience while staying compatible with Window Maker's lightweight
philosophy. It builds on the recent scaling, easing, and documentation updates,
and it details how to layer translucency, depth, and lively motion across the
stack.

## 1. Visual pillars

* **Vibrant translucency** – background materials should pick up wallpaper hues
  while remaining legible. Sonoma relies on live blurred backdrops with dynamic
  saturation tweaks instead of static alpha blends.
* **Depth through shadows and parallax** – window elements float above one
  another via soft drop shadows, gradients, and motion parallax when dragging or
  activating spaces.
* **Expressive motion** – animations use physically informed easing curves and
  spring dynamics so transitions feel responsive rather than mechanical.

The remaining sections map these pillars to concrete modules inside the
codebase.

## 2. Raster groundwork (wrlib)

1. **Material pipelines** – extend `wrlib/effects.c` with helpers that generate
   layered translucent materials: a blurred backdrop sampled from the current
   root pixmap, a saturation/brightness adjustment pass, and an optional
   highlight overlay. Return the composition as an `RImage` with premultiplied
   alpha so menus, panels, and tiles can reuse it.
2. **Live background sampling** – expose an API such as
   `RCreateBackdropMaterial(Display *dpy, Drawable root, const RRect *frame,
   const RMaterialSpec *spec)` that reuses the compositor pipeline above.
   Callers will hand in screen coordinates; the helper should grab pixels via
   `XGetImage`, feed them through `RBlurImage()`/`RSampleImageSmooth()`, apply
   saturation curves, and cache results for quick reuse during drags.
3. **Shadow and glow kernels** – add separable Gaussian kernels and configurable
   color stops so WINGs can request soft drop shadows or focus glows. Factor the
   kernels into `RGenerateShadow(RShadowSpec *)` utilities and reuse the existing
   transition curves to modulate opacity over time.
4. **Performance safeguards** – provide dirty-region aware invalidation hooks so
   the compositor only recomputes blurred materials when the wallpaper or window
   underneath changes, keeping CPU usage manageable on low-power systems.

## 3. Toolkit integration (WINGs)

1. **Material-aware widgets** – modify menu, panel, and inspector painters
   (`WINGs/wmenu.c`, `WINGs/wpanel.c`, `WINGs/wview.c`) to request translucent
   materials from wrlib instead of drawing flat gradients. Respect fallback
   defaults for themes that prefer opaque panels.
2. **Rounded corners and masks** – leverage the mask helpers outlined in the
   glass roadmap so every widget can clip to rounded rects before compositing the
   Sonoma material. Ensure the radius scales with widget size to avoid jagged
   edges.
3. **Dynamic shadows** – wrap existing bevel drawing with calls to the new
   shadow kernels. Provide theme keys such as `SonomaShadowRadius` and
   `SonomaShadowOpacity` inside `WINGs/WINGsDefaults.c` so WPrefs can surface the
   controls.
4. **Motion hooks** – extend interaction code (`wbutton.c`, `wscrollview.c`) to
   use the shared transition curves for hover and press effects. For scroll
   views, blend in the parallax offset by adjusting content transform matrices in
   response to scroll delta.

## 4. Core Window Maker updates

1. **Menu compositor** – in `src/menu.c`, replace the flat background fill with
   a call to the new wrlib material API, passing screen coordinates for each
   popup. Cache and reuse results across exposures to keep menus responsive.
2. **Window frames** – augment `src/framewin.c` and `src/framewin.h` with
   Sonoma-specific frame styles: translucent titlebars, rounded corners, and
   dynamic shadows. Hook the easing curves into `doWindowMove()` (already using
   them for drags) so parallax and fade animations stay consistent.
3. **Mission-control-like spreads** – reuse the transition curves and shadow
   helpers to animate workspace changes. When the user switches workspaces,
   fade and scale windows slightly, borrowing cues from Sonoma's spaces switcher
   without implementing a full compositor.
4. **Dock and appicons** – update `src/dock.c` and `src/icon.c` to request
   Sonoma materials for tiles and to animate launches using the shared easing
   helpers. Support larger icon sizes by default to match Sonoma's bolder
   imagery, while allowing fallbacks for classic themes.

## 5. Preferences exposure (WPrefs.app)

1. **Sonoma appearance panel** – create a dedicated panel that surfaces material
   intensity, corner radius, shadow softness, and motion strength controls.
   Persist selections via new defaults such as `SonomaMaterialStyle`,
   `SonomaCornerRadius`, and `SonomaShadowSpec`.
2. **Live previews** – reuse the icon scaling preview infrastructure to render a
   miniature panel that pulls actual wallpaper pixels (via the wrlib API) and
   updates in real time as the user moves sliders. The preview should reflect
   window frames, menus, and dock tiles so users understand the global impact.
3. **Accessibility toggles** – include checkboxes to reduce motion or disable
   translucency. These map to defaults consumed by both WINGs (for widgets) and
   the core window manager (for drag/launch animations).

## 6. Dockapps and GNUstep alignment

1. **Dockapp SDK notes** – document how dockapps can query the Sonoma defaults
   via wmlib (`WApplicationGetDockHints`) and render against the shared material
   textures. Provide example code that fetches the wallpaper sample and draws a
   translucent background before painting app content.
2. **GNUstep harmony** – coordinate with GNUstep's theming keys so applications
   that run under Window Maker inherit the same translucency and corner radius
   values. If GNUstep exposes NSAppearance-like hooks, map the Sonoma defaults to
   those toggles for consistency.

## 7. Implementation order and testing

1. Land the wrlib material and shadow APIs with unit tests that cover caching and
   performance safeguards.
2. Update WINGs widgets incrementally, starting with menus and panels before
   tackling more complex controls like scroll views.
3. Wire the core window manager to request materials and adopt the new defaults,
   verifying that legacy themes continue to work.
4. Build the WPrefs panel once the defaults schema is stable so testers can
   iterate on settings without recompiling.
5. Finish by updating documentation and sample themes, then exercise the build on
   multiple hardware profiles (low-power laptops, HiDPI displays) to validate
   responsiveness.

Following this blueprint keeps the Sonoma aesthetic grounded in reusable
infrastructure: wrlib supplies vibrant materials, WINGs presents them through
widgets, the core window manager animates them, and WPrefs lets users fine-tune
the experience.
