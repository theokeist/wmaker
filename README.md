# Window Maker

[![License: GPL v2 / LGPL v2](https://img.shields.io/badge/License-GPL_v2%20%7C%20LGPL_v2-blue.svg)](COPYING)
[![Platform: X11 / Linux / BSD](https://img.shields.io/badge/Platform-X11%20%7C%20Linux%20%7C%20BSD-lightgrey.svg)](#)
[![Standards: EWMH / ICCCM / GNOME / Motif](https://img.shields.io/badge/Standards-EWMH%20%7C%20ICCCM%20%7C%20Motif-success.svg)](#)
[![Compositor: Compton / Picom / XRender / GLX](https://img.shields.io/badge/Compositor-Compton%20%7C%20Picom%20%7C%20GLX-orange.svg)](#)
[![Status: Actively Maintained](https://img.shields.io/badge/Status-Actively%20Maintained-brightgreen.svg)](#)

**Window Maker** is an ultra-fast, lightweight, and extensible window manager for the **X Window System (X11)**. Originally crafted to emulate the timeless, elegant look and feel of the **NeXTSTEP™ / OpenStep** interface, Window Maker delivers a distraction-free desktop environment with native **GNUstep** integration, comprehensive **EWMH / NetWM** compliance, and modern compositing / visual enhancements.

---

## 📑 Table of Contents

1. [🌟 Complete Feature Matrix & Capabilities](#-complete-feature-matrix--capabilities)
   - [1. Compositor Integration & Visual Effects](#1-compositor-integration--visual-effects)
   - [2. Vim-Style Window Marking & Directional Focus](#2-vim-style-window-marking--directional-focus)
   - [3. Multi-Key Sequences & Sticky Chains](#3-multi-key-sequences--sticky-chains)
   - [4. Multi-Monitor, RandR & Hotplug Intelligence](#4-multi-monitor-randr--hotplug-intelligence)
   - [5. Next-Gen Image Engine (`wrlib`) & JPEG XL](#5-next-gen-image-engine-wrlib--jpeg-xl)
   - [6. Desktop Hot Corners & Screenshot Engine](#6-desktop-hot-corners--screenshot-engine)
   - [7. WINGs Widget Toolkit & Modern Typography](#7-wings-widget-toolkit--modern-typography)
   - [8. Modern Glass UI Architecture Blueprint](#8-modern-glass-ui-architecture-blueprint)
2. [🚀 Installation & Multi-Distribution Guides](#-installation--multi-distribution-guides)
3. [🖥️ Starting Window Maker](#️-starting-window-maker)
4. [⚙️ Configuration Reference & WPrefs Suite](#️-configuration-reference--wprefs-suite)
5. [📚 Specialized Documentation Index](#-specialized-documentation-index)
6. [📄 License & Authors](#-license--authors)

---

## 🌟 Complete Feature Matrix & Capabilities

### 1. Compositor Integration & Visual Effects

Window Maker features native support for X11 compositors (**Picom**, **Compton**, **xcompmgr**, and **Compiz**) with full management from the new **Effects** panel in `WPrefs.app`:

- **Automated Configuration Templates**: Automatically bootstraps `~/.config/picom/picom.conf` or `~/.config/compton.conf` if missing.
- **Backend Selection**: Seamlessly toggle between **GLX** (hardware-accelerated OpenGL) and **XRender** backends.
- **Dock & Clip Transparency Slider**: Granular 0%–100% opacity controls (`DockOpacity`), allowing background wallpapers to show through tiles and clips with modern frosted aesthetics.
- **Window Animations & Transitions**:
  - Configurable movement and launch transitions: **Classic** (legacy acceleration), **Smooth** (ease-in/out), and **Gentle** (smoothstep).
  - **Glide Iconification**: Smooth sliding minimization animations.
  - Live window content rendering during interactive drags (`ShowContentWhileMoving`) and resizes (`ShowContentWhileResizing`).

---

### 2. Vim-Style Window Marking & Directional Focus

Navigate complex multi-window workspaces without touching the mouse:

- **Vim-Inspired Window Marking**:
  - Assign arbitrary key labels to windows (e.g., `Super+M x` marks focused window with `x`).
  - **Jump (`MarkJumpKey`)**: Focus and raise the labeled window immediately.
  - **Bring (`MarkBringKey`)**: Teleport the marked window to your active workspace.
  - **Swap (`MarkSwapKey`)**: Swap positions and focus between the active window and marked window.
  - **Persistence**: All marks survive restarts and are saved in `~/GNUstep/Defaults/WMState`.
- **Directional Window Focus**:
  - Move keyboard focus topologically (`FocusWindowLeftKey`, `FocusWindowRightKey`, `FocusWindowUpKey`, `FocusWindowDownKey`) using window center geometric vectors.
- **Universal Window Cycling**:
  - Alt-Tab switcher can optionally cycle windows across **all** virtual workspaces (`CycleAllWorkspaces = YES`).

---

### 3. Multi-Key Sequences & Sticky Chains

- **Prefix-Based Keybindings**: Supports Emacs/tmux-style leader-key sequences (e.g., `<Leader> -> w -> v` for vertical split/tile).
- **Sticky-Chain Mode**: Keep prefix states open across rapid successive commands with configurable expiration timeout (`KeychainTimeoutDelay`, default $500\text{ms}$) and explicit cancellation (`KeychainCancelKey`).
- **Customizable Unicode Modifier Labels (`ModifierKeyShortLabels`)**: Replace standard text abbreviations with macOS-style glyphs (⌘, ⌥, ⇧, ⌃) or custom symbols in menus.

---

### 4. Multi-Monitor, RandR & Hotplug Intelligence

- **Dynamic Head Snap (`wClipSnapToHead`)**: When monitors are hotplugged or disconnected, Clips and AppIcons automatically compute their relative coordinates and snap into visible heads without clipping off-screen.
- **Hotplug Monitor Auto-Activation (`HotplugMonitor`)**: Automatically detects and activates newly plugged displays, expanding your desktop to the right.
- **Multi-Head Dock Pinning (`KeepDockOnPrimaryHead`)**: Pins the main application dock to the primary display head across dynamic layout adjustments.
- **EWMH `_NET_WM_MOVERESIZE` Support**: Full compatibility with client-side decorated applications (VS Code, Google Chrome, Discord, Steam) for interactive moving and resizing.
- **Multi-Head Fullscreen (`_NET_WM_FULLSCREEN_MONITORS`)**: Multi-display spanning for video players and presentations.

---

### 5. Next-Gen Image Engine (`wrlib`) & JPEG XL

- **JPEG XL (JXL) Support**: Direct native loading of next-generation `.jxl` images via `libjxl`.
- **Comprehensive Image Support**: WebP, PNG, JPEG, TIFF, GIF, XPM, PNM, and SVG.
- **In-Memory Image Rasterization**: Fast in-memory image encode/decode pipelines that reduce disk thrashing during theme and icon switches.
- **Catmull-Rom Scaling**: High-fidelity cubic interpolation for crystal-clear icon and thumbnail rendering.
- **`wmiv` Image Viewer Upgrades**:
  - Direct archive reading (view images inside `.zip`, `.tar.gz`, `.7z` via `libarchive`).
  - Copy current image directly to clipboard (`Ctrl+C`).
  - Graceful handling and filtering of unsupported formats.

---

### 6. Desktop Hot Corners & Screenshot Engine

- **Programmable Hot Corners (`HotCorners`)**:
  - Assign distinct actions or shell scripts to the 4 screen corners (Top-Left, Top-Right, Bottom-Left, Bottom-Right).
  - Configurable edge sensitivity radius (`HotCornerEdge`, 2–10px) and trigger activation delay (`HotCornerDelay`, default $250\text{ms}$).
- **Built-in Screenshot Engine**:
  - Three capture modes: **Entire Screen**, **Active Window**, or **Interactive Selection Rectangle**.
  - Customizable `strftime` naming templates (`ScreenshotFileNameTemplate`) saved directly to `~/GNUstep/Library/WindowMaker/Screenshots/`.

---

### 7. WINGs Widget Toolkit & Modern Typography

- **Multi-Tier Font Fallback**: Dynamic fallback cascade preventing crashes when specialized legacy X11 fonts are absent.
- **Ghosting & Caret Drift Elimination**: Completely rebuilt text-selection and caret-tracking calculations in `wtextfield`.
- **Unbroken UTF-8 Titlebars & Menus**: Unicode string sanitization ensuring right-to-left and multibyte titles never truncate improperly.
- **Titlebar Language Switcher**: Compact XKB layout switcher button cycling through active keyboard language groups on click.

---

### 8. Modern Glass UI Architecture Blueprint

Comprehensive roadmap and design notes ([`doc/GLASS_AND_MODERN_UI_NOTES.md`](doc/GLASS_AND_MODERN_UI_NOTES.md)):
- Translucent rounded window borders and titlebars.
- Frosted acrylic popovers with subtle inset lighting highlights.
- High-DPI icon assets and scalable SVG workflow.

---

## 🚀 Installation & Multi-Distribution Guides

### 1. Install Dependencies

#### Debian / Ubuntu / Devuan / Linux Mint
```bash
sudo apt update
sudo apt install build-essential autoconf automake libtool pkg-config \
    libx11-dev libxext-dev libxft-dev libxinerama-dev libxrandr-dev \
    libxmu-dev libxpm-dev libpng-dev libjpeg-dev libtiff-dev libgif-dev \
    libwebp-dev libjxl-dev libfontconfig1-dev libfreetype6-dev \
    libxres-dev libxkbfile-dev compton
```
*(Detailed guide: [`doc/BUILDING_ON_DEBIAN_UBUNTU.md`](doc/BUILDING_ON_DEBIAN_UBUNTU.md))*

#### Arch Linux / CachyOS / Manjaro
```bash
sudo pacman -S base-devel libx11 libxext libxft libxinerama libxrandr \
    libxmu libxpm libpng libjpeg-turbo libtiff giflib libwebp libjxl \
    fontconfig freetype2 libxres libxkbfile picom
```

#### PCLinuxOS / ALT Linux / Fedora / openSUSE
*(Detailed guide: [`doc/BUILDING_ON_PCLINUXOS_ALT.md`](doc/BUILDING_ON_PCLINUXOS_ALT.md))*

---

### 2. Build & Install

```bash
# Clone repository
git clone git@github.com:theokeist/wmaker.git
cd wmaker

# Generate build configuration
./autogen.sh

# Configure with modern features enabled
./configure \
    --prefix=/usr/local \
    --enable-modelock \
    --enable-randr \
    --enable-xinerama \
    --enable-jxl \
    --enable-webp

# Compile using all available CPU cores
make -j$(nproc)

# Install binaries, manual pages, and icons
sudo make install
sudo ldconfig
```

---

## 🖥️ Starting Window Maker

### 1. Display Managers (GDM, LightDM, SDDM)
Window Maker provides `/usr/share/xsessions/wmaker.desktop`. Simply select **Window Maker** in your display manager session dropdown.

### 2. Console Start (`startx` / `.xinitrc`)
Add the following to your `~/.xinitrc`:
```bash
#!/bin/sh

# Set keyboard layout (optional)
setxkbmap us,cz

# Launch compositor daemon in background
compton -b &

# Start Window Maker
exec wmaker
```
*(Comprehensive session management guide: [`doc/RUNNING_WMAKER.md`](doc/RUNNING_WMAKER.md))*

---

## ⚙️ Configuration Reference & WPrefs Suite

All user preferences are organized under `~/GNUstep/Defaults/`:

| File | Description |
|---|---|
| `WindowMaker` | Core settings: focus modes, animations, keybindings, compositor toggles, dock opacity. |
| `WMRootMenu` | Applications root menu hierarchy, submenu cascades, and execution commands. |
| `WMWindowAttributes` | Per-window specifications (e.g. AlwaysOnTop, Borderless, SkipWindowList). |
| `WMState` | Workspace definitions, docked app positions, clip states, and Vim-style window marks. |
| `compton.conf` | Compositor parameters (shadow blur radius, opacity rules, fading speed). |

### Graphical Preferences Suite (`WPrefs.app`)
Launch `WPrefs` from the desktop menu or terminal to graphically customize:
- **Effects & Compositor**: Backend switching, dock transparency, window animation curves.
- **Window Handling**: Focus policies (Click-to-Focus, Sloppy, Focus-Follows-Mouse), auto-raise delays.
- **Expert Preferences**: Mouse wheel focus, multi-workspace cycling, monitor hotplugging.
- **Keyboard Shortcuts**: Multi-key sequences, directional focus keys, screenshot shortcuts.
- **Theme & Appearance**: Textures, titlebar gradients, font smoothing, and color schemes.

---

## 📚 Specialized Documentation Index

- [`doc/RUNNING_WMAKER.md`](doc/RUNNING_WMAKER.md) — Running Window Maker via xinitrc, display managers, and sessions.
- [`doc/BUILDING_ON_DEBIAN_UBUNTU.md`](doc/BUILDING_ON_DEBIAN_UBUNTU.md) — Step-by-step Debian 12/13 & Ubuntu 25.10 build instructions.
- [`doc/BUILDING_ON_PCLINUXOS_ALT.md`](doc/BUILDING_ON_PCLINUXOS_ALT.md) — RPM distro compilation and packaging guide.
- [`doc/GLASS_AND_MODERN_UI_NOTES.md`](doc/GLASS_AND_MODERN_UI_NOTES.md) — Glass UI design architecture and wrlib materials blueprint.
- [`doc/GS_DESKTOP_POLISHING.md`](doc/GS_DESKTOP_POLISHING.md) — GNUstep workspace polishing and styling.
- [`doc/WINGS_APP_WORKFLOW.md`](doc/WINGS_APP_WORKFLOW.md) — Building lightweight X11 GUI applications with WINGs.
- [`FAQ`](FAQ) — Comprehensive Frequently Asked Questions & Troubleshooting.

---

## 📄 License & Authors

- **License**: Window Maker is licensed under the **GNU General Public License (GPL v2 or later)**. Accompanying libraries (`WINGs`, `wrlib`, `wmlib`) are licensed under the **GNU Lesser General Public License (LGPL v2 or later)**.
- **Original Authors**: Alfredo K. Kojima, Dan Pascu, Banlu Kemiyatorn, and the Window Maker Team.
- **Repository**: [theokeist/wmaker](https://github.com/theokeist/wmaker) (synchronized with [window-maker/wmaker](https://github.com/window-maker/wmaker)).
