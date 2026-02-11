# GNUstep Desktop Polishing Ideas

## Context
The GNUstep Desktop (gs-desktop) experiments with richer desktop polish on top of Window Maker and WINGs. Its patches demonstrate how incremental ergonomics work—like smoother icon handling, animated transitions, and modernized preferences—can make the classic NeXT-inspired environment feel more contemporary without rewriting the entire stack. This guide outlines similar polish that could be upstreamed or reimplemented in Window Maker while respecting the existing architecture.

## Raster and Effects (wrlib)
- **Shared easing curves:** Continue generalizing the easing helper added in `wrlib/effects.c` so every animation primitive (icon zoom, window slide, fade) is parameterized through defaults. gs-desktop uses comparable tables to align motion curves across components; mirroring that approach keeps dock launches, workspace flips, and menu fades visually consistent.
- **High-quality sampling:** Introduce Lanczos or bicubic down-scaling paths alongside the current smooth/fast choices. gs-desktop ships higher DPI icon themes, and retaining detail when shrinking them is crucial for clean presentation on dense displays.
- **Compositable overlays:** Provide helpers that blend highlight glows or drop shadows into pixmaps before they reach X11. This keeps the effects opt-in but lets WPrefs toggle them without each caller duplicating blending math.

## Core Window Maker
- **Window choreography:** Expose the new easing defaults surfaced by `WPrefs.app/Effects.c` for move/resize loops, not just dock animations. gs-desktop patches showcase subtle acceleration when dragging windows that reduces perceived lag; adopting similar hooks would reuse the shared transition curves.
- **Workspace and clip polish:** Allow per-workspace background transitions and clip/dock auto-hide animations to opt into the same easing catalog. This keeps right-click menus, dock tiles, and workspace switches feeling part of one design system.
- **Defaults schema cleanup:** Group the new animation and sampling toggles under a dedicated `VisualEffects` dictionary in the defaults database so WPrefs, dockapps, and external tools can query capabilities without guessing key names.

## WINGs Toolkit
- **Scalable widgets:** Audit scroll views, inspectors, and icon buttons to respect the icon-size slider introduced in WPrefs (`WPrefs.app/Icons.c`). gs-desktop stretches inspectors for HiDPI monitors; upstream parity would entail making WINGs widgets query theme metrics instead of hard-coding pixel sizes.
- **Smooth scrolling:** Route scrollwheel events through easing-aware timers before updating the viewport, so scroll views glide instead of stepping. This mirrors how gs-desktop softens long lists in its preferences and dock expanders.
- **Theme channels:** Extend the theme loader so gradients, borders, and shadows can describe multi-stop ramps or blur radii, aligning with the richer gs-desktop skins. WINGs would parse these values and hand them to wrlib’s blending helpers.

## WPrefs.app
- **Effect inspector:** Expand the dedicated Animations & Effects panel (`WPrefs.app/Effects.c`) to preview easing curves side-by-side, similar to gs-desktop’s control center. Embedding the raster helper lets users see launch, close, and workspace transitions before applying them globally.
- **Icon lab:** Complement the icon-size slider (`WPrefs.app/Icons.c`) with per-source sampling toggles (fast vs. smooth vs. high-quality). gs-desktop exposes this distinction to balance performance on low-end hardware.
- **Accessibility modes:** Offer preset bundles (e.g., “Reduced Motion,” “High Contrast”) that flip the relevant defaults in one click, inspired by gs-desktop’s accessibility panel. WPrefs can then write consistent defaults keys for dockapps to honor.

## Dockapps and External Utilities
- **Minimal dependencies:** Keep dockapps simple—gs-desktop demonstrates that polished animations can live in Window Maker itself while dockapps remain lightweight. Encourage dockapps to rely on wrlib for any scaling so they inherit the global sampling improvements without bundling extra code.
- **Status overlays:** Provide optional hooks for dockapps to register overlay badges (e.g., unread counts) rendered via wrlib’s compositing helper. This keeps dockapps focused on data while the core ensures visual consistency.

## Integration Strategy
1. **Land shared primitives first** in wrlib and the defaults schema.
2. **Adopt the primitives** in core animation paths (dock, clip, window motion).
3. **Expose configuration** through WPrefs with previews powered by the same helpers.
4. **Document expectations** for dockapps and external utilities so they piggyback on the new polish without reinventing it.

Following this progression lets Window Maker absorb gs-desktop’s refinements methodically, ensuring each layer benefits from the shared infrastructure and giving users visible improvements—especially in right-click menus, window motion, and icon clarity—after rebuilding the project.
