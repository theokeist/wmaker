# Glass and Modern UI Modernization Guide

This guide merges the earlier glass/skeuomorphic roadmap and the Modern UI blueprint
into a single reference. It explains how to evolve Window Maker's visuals—from rounded
menus to vibrant translucent materials—while keeping the codebase lightweight and
backward compatible. Each section calls out the relevant subsystems (wrlib, WINGs, the
core window manager, defaults plumbing, WPrefs, and dockapps/GNUstep clients) so work
can be planned and executed without surprises.

## 1. Visual pillars and goals

* **Tactile depth** – rounded corners, soft shadows, and gloss details create a more
  physical UI without abandoning the classic NeXT-inspired feel.
* **Vibrant translucency** – materials sample the current wallpaper, applying blur and
  saturation adjustments so menus and panels pick up ambient color while remaining
  legible. An external compositor (e.g., `picom`) performs the final blend.
* **Expressive motion** – shared transition curves drive window moves, launches, and
  hover effects so the UI feels responsive and cohesive across both classic and
  modern themes.

The following sections map these goals to concrete engineering tasks.

## 2. Raster foundations (wrlib)

1. **Material & mask helpers** – extend `wrlib/scale.c` and `wrlib/effects.c` with
   utilities that can generate rounded masks, drop-shadow falloffs, and layered
   translucent materials. APIs such as `RMakeRoundedFrame()` and
   `RCreateBackdropMaterial()` should return premultiplied `RImage` buffers so callers
   can composite them safely when a compositor is active.
2. **Live background sampling** – expose helpers that grab wallpaper pixels using
   `XGetImage`, run them through blur/saturation passes, and cache results for quick
   reuse during drags. Provide dirty-region invalidation so the sampling cost stays
   manageable on low-power systems.
3. **Shadow and glow kernels** – add separable Gaussian kernels and configurable color
   stops (`RGenerateShadow(RShadowSpec *)`) so WINGs can request soft shadows or focus
   glows. Use the shared transition curves to modulate opacity over time.
4. **High-DPI aware scaling** – build on `RScaleImageSmooth()` so large radii and blur
   kernels remain crisp. When reducing oversized imagery, always favor smooth scaling
   to prevent jagged edges on retina displays.
5. **ARGB visual selection** – update `wrlib/context.c` so it prefers a 32-bit,
   alpha-capable visual whenever the Composite extension is available. Fall back to
   the current TrueColor path when necessary to preserve compatibility.

## 3. Toolkit hooks (WINGs)

1. **Rounded clipping & materials** – modify painters in `WINGs/wmenu.c`,
   `WINGs/wpanel.c`, `WINGs/wview.c`, and `WINGs/wbutton.c` to accept optional rounded
   masks and translucent materials from wrlib. Apply the mask just before bevel or border strokes so corners stay clean.
2. **Scrollbar and scrolling polish** – refresh `WINGs/wscrollview.c` so scrollbars can
   adopt larger thumb textures, inherit container radii, and use transition curves for
   hover/drag feedback. Consider subtle parallax when scrolling content to reinforce
   depth.
3. **Defaults schema** – extend `WINGs/WINGsDefaults.c` with keys such as
   `MenuCornerRadius`, `GlassHighlightOpacity`, `ModernShadowRadius`, and motion/toggle
   options. Default them to classic values so older themes render unchanged.
4. **Compositor-aware windows** – when an ARGB visual is active, create widget backing
   windows with the matching colormap and cache ARGB pixmaps for off-screen painting.
   Replace opaque `XFillRectangle` paths with wrlib blits that honor premultiplied
   alpha before presenting the final surface.
5. **Graceful degradation** – detect when no compositor is present and fall back to
   the traditional opaque theme so the modernization does not regress classic setups.

## 4. Core Window Maker integration

1. **Menus and root menu** – update `src/menu.c` so each `WMenu` requests rounded masks
   and translucent materials, reusing indicator glyphs (`MI_*` types) without clipping
   labels. Cache materials between exposures to maintain responsiveness.
2. **Window frames and motion** – extend `src/framewin.c`/`framewin.h` with
   per-corner radius support, translucent titlebars, and dynamic shadows. Ensure
   `doWindowMove()` preserves masks and leverages transition curves for parallax or
   fade animations.
3. **Dock tiles and appicons** – adjust `src/dock.c` and `src/icon.c` to apply the same
   rounded masking helpers and material pipelines to application tiles. Provide
   defaults so legacy square themes remain untouched when users opt out.
4. **Workspace transitions** – explore Mission Control-style spreads by reusing the
   easing curves and shadow helpers when switching workspaces, keeping effects light
   enough for systems without a compositor.
5. **Configuration plumbing** – extend `src/defaults.c`, `src/wdefaults.c`, and
   `src/WindowMaker.h` with the new keys while preserving compatibility with existing
   preference files.

## 5. Preferences and user control (WPrefs.app)

1. **Appearance panels** – enhance the Appearance controls and the dedicated
   Animations & Effects panel with sliders for corner radius, shadow depth,
   material intensity, and motion strength. Store selections using the schema keys
   defined above so both classic and modern looks stay configurable from WPrefs.
2. **Live previews** – reuse the icon preview infrastructure in `WPrefs.app/Icons.c`
   to render miniature panels/menus that demonstrate the active material and easing
   settings. When possible, sample actual wallpaper pixels via the wrlib helpers so
   previews match on-screen results.
3. **Accessibility toggles** – surface options to reduce motion or disable
   translucency. These map directly to defaults consumed by both WINGs (for widget
   rendering) and the core window manager (for drag/launch animations).
4. **Feedback loop** – trigger `WMSetLookChanged()` whenever key settings change so the
   running window manager reloads defaults without requiring a restart.

## 6. Dockapps, GNUstep, and ecosystem alignment

1. **Dockapp guidance** – document how dockapps can query the new defaults via wmlib
   (`WApplicationGetDockHints` and related helpers) and render against shared masks or
   materials. Encourage shipping higher-resolution assets so scaling to larger radii
   stays sharp.
2. **GNUstep harmony** – mirror key defaults with GNUstep theme keys
   (`GSThemeCornerRadius`, translucency toggles, etc.) so GNUstep panels align with the
   Window Maker look when both run side by side.
3. **Client opt-in** – provide sample code showing how third-party apps detect whether
   the compositor-friendly path is active and gracefully fall back when running on
   classic setups.

## 7. External compositor interplay

Window Maker still relies on a separate compositor to blend translucent pixels onto the
framebuffer. To make the most of that compositor:

1. **Autostart guidance** – recommend launching a compositor (e.g., `picom`) via
   `~/GNUstep/Library/WindowMaker/autostart` so it runs alongside Window Maker.
2. **Premultiplied surfaces** – ensure wrlib returns buffers flagged as premultiplied
   alpha and offer helpers that upload them to XRender pictures for reuse across
   widgets.
3. **Fallback detection** – if no compositor is present, skip translucency and render
   using the existing opaque theme path to avoid visual glitches.
4. **Glide animations** – the new *Glide* iconification style drives an ARGB overlay
   window with `_NET_WM_WINDOW_OPACITY`, letting compositors fade snapshots smoothly
   during minimize/restore. Without a compositor the fallback outline path keeps
   animations responsive without translucency.

## 8. Implementation order and validation

1. Land the wrlib material/mask/shadow APIs with unit tests covering caching and
   performance safeguards.
2. Update WINGs widgets incrementally—start with menus/panels, then tackle scroll
   views and buttons—verifying both ARGB and classic paths.
3. Wire the core window manager to request the new assets, confirming that legacy
   themes remain functional.
4. Expand WPrefs once the defaults schema stabilizes so testers can tweak settings
   without recompiling.
5. Exercise the build across hardware profiles (low-power laptops, HiDPI displays)
   and with/without an external compositor to validate responsiveness and fallback
   behavior.

## 9. Visual QA checklist

After implementing the modernization work, rebuild and confirm:

1. **Menus** – right-click menus display rounded corners, glass gloss, and dynamic
   icons without clipping text.
2. **Window frames** – focused/unfocused frames retain curved edges, gradients, and
   transition-driven animations during move/resize.
3. **Scroll views** – WINGs apps (WPrefs, dockapp utilities) show smoother scrolling
   with rounded container edges and consistent highlight behavior.
4. **Dock tiles** – appicons and dockapps receive the rounded/material treatment while
   classic square themes remain available when the new defaults are disabled.
5. **High-DPI** – Retina/HiDPI displays render crisp corners, shadows, and materials
   thanks to the wrlib scaling improvements.

Following this unified guide keeps glass-era skeuomorphism and Modern-inspired
translucency grounded in reusable infrastructure: wrlib supplies the image primitives
and materials, WINGs exposes them to applications, the core window manager consumes
them for chrome and motion, and WPrefs lets users dial in their preferred balance of
modern flair and classic efficiency.
