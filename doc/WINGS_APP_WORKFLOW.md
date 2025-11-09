# WINGs Application Workflow

This guide captures how the in-tree widget toolkit fits into Window Maker’s
architecture and how applications such as WPrefs.app orchestrate their user
interface with it.

## Design intent

WINGs was created as a compact widget set that echoes the NeXTSTEP look while
remaining fast, low-level, and tightly scoped to Window Maker’s needs. Its API
closely mirrors OpenStep classes, providing the essential controls required for
tools like WPrefs without aspiring to be a full desktop framework. The upstream
README stresses that larger or highly ornate programs should target GNUstep
instead, keeping WINGs focused on small utilities and Window Maker support.
【F:WINGs/README†L1-L39】

## Application responsibilities

Because WINGs supplies windows, panels, buttons, lists, tab views, sliders,
notifications, defaults storage, and drag-and-drop primitives, a typical
application arranges these widgets to expose the settings or workflows it needs.
Widget state is synchronized with Window Maker’s defaults system, either through
WINGs’ user defaults helpers or by writing to the GNUstep defaults domain.
【F:WINGs/README†L21-L64】

The toolkit expects callers to drive the event loop and manage layout
explicitly—giving experienced Xlib developers room to optimize behavior, while
still offering enough convenience to build preference panels, dockapp
configuration windows, and small utilities without reimplementing basic widgets.

## Case study: WPrefs.app

WPrefs serves as the canonical WINGs consumer inside the tree. It exposes most of
Window Maker’s preference options through tabbed panels, sliders, and list views,
allowing users to manipulate menus, icons, and runtime behavior from a GUI. The
README outlines how it should be launched from its GNUstep application bundle and
how it persists preferences into the user’s defaults domain, reinforcing the
workflow of editing settings, saving them, and letting Window Maker pick them up
on restart or live reload.【F:WPrefs.app/README†L1-L57】

Recent updates extend that workflow to animation: the dedicated
**Animations & Effects** panel now writes easing choices, compositor
preferences, autostart and window-shadow toggles, and content-capture settings,
and the core
window manager reads those defaults to feed the shared wrlib transition
curves—keeping UI tweaks, Picom integration, and rendering ergonomics
aligned.【F:WPrefs.app/Effects.c†L1-L520】

When the app starts, WINGs constructs the application icon, registers it with the
dock, and opens panels corresponding to each configuration section. Users can
leave it running in the background—WINGs keeps the windows hidden until they are
reactivated—making it easy to iterate on appearance tweaks or menu edits without
paying startup cost every time.【F:WPrefs.app/README†L17-L33】

## Working alongside GNUstep and dockapps

Window Maker positions itself as the official window manager for GNUstep, so
WINGs coexists with the richer Objective-C toolkits rather than replacing them.
Complex applications are expected to lean on GNUstep’s AppKit, while WINGs
remains the lightweight choice for utilities shipped with the window manager.
【F:README†L38-L56】

Dockapps can pick whichever stack fits their scope. Many use WINGs for quick
controls or menus; others talk directly to Xlib or adopt GNUstep if they need
more advanced views. Regardless of the choice, wmlib’s helper routines and
Window Maker’s defaults system give them consistent hooks for registering icons,
listening to events, and integrating with the dock.【F:wmlib/WMaker.h†L108-L156】
