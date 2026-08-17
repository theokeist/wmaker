# Window Maker

[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](COPYING)
[![Platform: X11 / Linux / BSD](https://img.shields.io/badge/Platform-X11%20%7C%20Linux%20%7C%20BSD-lightgrey.svg)](#)
[![Standards: EWMH / ICCCM / GNOME / Motif](https://img.shields.io/badge/Standards-EWMH%20%7C%20ICCCM%20%7C%20Motif-success.svg)](#)
[![Status: Actively Maintained](https://img.shields.io/badge/Status-Actively%20Maintained-brightgreen.svg)](#)

**Window Maker** is an ultra-fast, lightweight, and highly configurable window manager for the **X Window System (X11)**. Originally designed to emulate the clean and elegant look-and-feel of the **NeXTSTEP™ / OpenStep** user interface, it provides a distraction-free desktop environment with native **GNUstep** integration, comprehensive **EWMH / NetWM** compliance, and modern visual enhancements.

---

## 🌟 Modern Enhancements & Feature Highlights

While preserving its legendary efficiency and sub-millisecond responsiveness, this repository branch incorporates modern desktop features and bug fixes:

### 🎨 1. Compositor Integration & Visual Effects
- **Built-in Compton / Picom Integration**: Automatic configuration template creation (`~/GNUstep/Defaults/compton.conf`) and daemon lifecycle management directly from the window manager.
- **WPrefs Effects Panel**: Graphical controls for compositor autostart, backend selection (**GLX** / **XRender**), window shadow radius/opacity, and fading transitions.
- **Dock & Clip Transparency**: Configurable per-tile opacity allowing backdrop wallpaper blending and modern glass-like dock styling.

### 💎 2. Modern UI & Glass Architecture
- **Glass / Rounded Theme Blueprint**: Dedicated architecture notes ([`doc/GLASS_AND_MODERN_UI_NOTES.md`](doc/GLASS_AND_MODERN_UI_NOTES.md)) documenting wrlib materials, translucent surfaces, and rounded corner geometry.
- **Non-Opaque Window Drag Optimization**: Smooth wireframe and alpha-blended motion handling.

### 🖼️ 3. Next-Gen Image Engine (`wrlib`)
- **JPEG XL (JXL) Support**: Native decoding for `.jxl` images alongside WebP, PNG, JPEG, TIFF, GIF, and XPM.
- **In-Memory Rasterization**: High-speed memory image encoding/decoding buffers to minimize disk I/O churn during theme switches.
- **Catmull-Rom Scaling**: High-quality default scaling interpolation algorithm for application icons and mini-windows.

### 🖥️ 4. Multi-Monitor & RandR Dynamic Reconfiguration
- **Dynamic Head Snap (`wClipSnapToHead`)**: Clips and dock modules automatically re-anchor onto visible display heads during dynamic monitor hotplugging and resolution changes without disappearing.
- **Multi-Head Dock Pinning**: KeepDockOnPrimaryHead support with coordinate persistence.

### 🔤 5. WINGs Toolkit Modernization
- **Font Fallback Mechanism**: Robust multi-level font fallback chain preventing fallback crashes on systems missing specific legacy X11 fonts.
- **Textfield Caret & Ghosting Fixes**: Resolved character offset drift, text wrapping anomalies, and redraw artifacts in `wtextfield`.
- **Full UTF-8 String Handling**: Unbroken UTF-8 window titles, icon captions, and right-to-left / multibyte character compatibility.

### ⌨️ 6. Advanced Keyboard Navigation
- **Multikey Keybinding Chains & Sticky Mode**: Emacs/Vim-style key sequences for window and workspace operations.
- **Dynamic Window Marking**: Quick jump and window-tagging capabilities.

---

## 🚀 Quick Start & Building

### Dependencies

Install the core X11, image, and development libraries:

- **Debian / Ubuntu / Devuan**:
  ```bash
  sudo apt install build-essential autoconf automake libtool pkg-config \
      libx11-dev libxext-dev libxft-dev libxinerama-dev libxrandr-dev \
      libxmu-dev libxpm-dev libpng-dev libjpeg-dev libtiff-dev libgif-dev \
      libwebp-dev libjxl-dev libfontconfig1-dev libfreetype6-dev compton
  ```
  *(See [`doc/BUILDING_ON_DEBIAN_UBUNTU.md`](doc/BUILDING_ON_DEBIAN_UBUNTU.md) for full walkthrough)*

- **Arch Linux / CachyOS / Manjaro**:
  ```bash
  sudo pacman -S base-devel libx11 libxext libxft libxinerama libxrandr \
      libxpm libpng libjpeg-turbo libtiff giflib libwebp libjxl fontconfig freetype2
  ```

- **PCLinuxOS / ALT Linux**:
  *(See [`doc/BUILDING_ON_PCLINUXOS_ALT.md`](doc/BUILDING_ON_PCLINUXOS_ALT.md))*

### Compilation & Installation

```bash
# 1. Generate build system scripts
./autogen.sh

# 2. Configure features
./configure --prefix=/usr/local --enable-modelock --enable-randr --enable-jxl

# 3. Build binaries
make -j$(nproc)

# 4. Install
sudo make install
sudo ldconfig
```

---

## 🖥️ Running Window Maker

### Via Display Manager (GDM, LightDM, SDDM)
Window Maker installs standard desktop session definitions:
```bash
/usr/share/xsessions/wmaker.desktop
```
Simply choose **Window Maker** from your login screen session menu.

### Via `startx` or `.xinitrc`
To launch Window Maker from a terminal console, add the following to `~/.xinitrc`:
```bash
# Start background services (optional)
compton -b &

# Launch Window Maker
exec wmaker
```
*(Read [`doc/RUNNING_WMAKER.md`](doc/RUNNING_WMAKER.md) for complete session recipes and migration tips)*

---

## ⚙️ Configuration & Tooling

Window Maker configurations reside in `~/GNUstep/Defaults/`:

| Configuration File | Purpose |
|---|---|
| `WindowMaker` | Core window manager behavior, workspace count, animations, and keybindings. |
| `WMRootMenu` | Root desktop menu hierarchy and application launch entries. |
| `WMWindowAttributes` | Per-application window rules (sticky, borderless, floating, miniaturize). |
| `compton.conf` | Visual compositor settings (shadows, blur, transparency). |

### Graphical Configuration Suite (`WPrefs.app`)
Run `WPrefs` from the root menu or terminal to graphically customize:
- Workspace animations & glide iconification
- Font anti-aliasing & typography
- Dock & Clip behaviors
- Keyboard shortcuts & multi-key chains
- Compositor visual effects

---

## 📚 Documentation Index

- [`doc/RUNNING_WMAKER.md`](doc/RUNNING_WMAKER.md) — Starting Window Maker via xinitrc, display managers, and sessions.
- [`doc/BUILDING_ON_DEBIAN_UBUNTU.md`](doc/BUILDING_ON_DEBIAN_UBUNTU.md) — Debian 12/13 and Ubuntu 25.10 compilation guide.
- [`doc/BUILDING_ON_PCLINUXOS_ALT.md`](doc/BUILDING_ON_PCLINUXOS_ALT.md) — RPM-based distros dependency setup.
- [`doc/GLASS_AND_MODERN_UI_NOTES.md`](doc/GLASS_AND_MODERN_UI_NOTES.md) — Glass UI and modern design architecture roadmap.
- [`doc/GS_DESKTOP_POLISHING.md`](doc/GS_DESKTOP_POLISHING.md) — GNUstep application integration details.
- [`doc/WINGS_APP_WORKFLOW.md`](doc/WINGS_APP_WORKFLOW.md) — Developing standalone apps with WINGs widget toolkit.
- [`FAQ`](FAQ) — Comprehensive Frequently Asked Questions.

---

## 📄 License & Attribution

- Window Maker is free software licensed under the **GNU General Public License (GPL v2 or later)**. See [`COPYING`](COPYING) for details.
- The `WINGs`, `wrlib`, and `wmlib` utility libraries are licensed under the **GNU Lesser General Public License (LGPL v2 or later)**.
- Original authors: **Alfredo K. Kojima**, **Dan Pascu**, and the Window Maker Developer Team.
- Official upstream repository: [window-maker/wmaker](https://github.com/window-maker/wmaker).
