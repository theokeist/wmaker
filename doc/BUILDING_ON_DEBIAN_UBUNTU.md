# Building Window Maker on Debian and Ubuntu

This guide captures the package sets and build steps that have been tested on
current Debian and Ubuntu releases. Follow the section that matches your system
and you will end up with a full developer toolchain plus the optional raster
libraries that give Window Maker richer image support.

## Shared preparation

Regardless of the distribution, start with an up-to-date package index and the
standard development meta-package:

```sh
sudo apt update
sudo apt install build-essential
```

The `build-essential` group pulls in GCC, `make`, the C runtime headers, and
other low-level pieces that the remaining steps assume are already present.

## Debian 12 (Bookworm) and Debian 13 (Trixie)

### 1. Install the development dependencies

Bookworm already ships everything Window Maker needs in the main archive. Install
the compiler toolchain, X11 headers, autotools required for the Git tree, and
common image libraries:

```sh
sudo apt install \
  autoconf automake autotools-dev libtool pkg-config git gettext \
  libx11-dev libxext-dev libxrandr-dev libxinerama-dev libxcursor-dev \
  libxrender-dev libxpm-dev libxmu-dev libsm-dev libice-dev \
  libfreetype6-dev libfontconfig1-dev libxft-dev \
  libpng-dev libjpeg-dev libtiff-dev libgif-dev libwebp-dev \
  libxcomposite-dev libxdamage-dev libxfixes-dev libxss-dev \
  mesa-utils libgl1-mesa-dev
```

All of the image libraries above are optional at compile time, but installing
them now lets Window Maker load the full set of background textures and icons
without configure warnings later.

If you prefer to rely on Debian's curated dependency list, you can also run:

```sh
sudo apt build-dep wmaker
```

This installs the toolchain and libraries that the packaged version was built
against, which aligns closely with the explicit package list above.

### 2. Fetch the source tree

```sh
git clone https://github.com/window-maker/wmaker.git
cd wmaker
```

### 3. Generate the configure script (Git checkouts only)

The Git repository ships `autogen.sh` to bootstrap the autotools build system:

```sh
./autogen.sh
```

If you are building from a release tarball, the configure script is already
present and you can skip this step.

### 4. Configure, build, and install

```sh
./configure --prefix=/usr/local
make -j"$(nproc)"
sudo make install
```

Using `/usr/local` keeps the build separate from Debian's packaged binaries. If
you want to stage everything under a temporary directory instead, pass
`--prefix="$PWD/stage"` and later copy the tree where you need it.

### 5. (Optional) Wire in compositor defaults and shadows

Window Maker's modern animations rely on an external compositor. Install one or
more compositor packages and choose a default backend at build time:

```sh
sudo apt install picom compton xcompmgr compiz
./configure --prefix=/usr/local --with-compositor=<picom|compton|xcompmgr|compiz|none>
```

Picom and Compton templates land in `/usr/local/share/WindowMaker/Compositors/` as `picom.conf` and `compton.conf`.
After installing, open **WPrefs → Animations & Effects**, pick your preferred compositor, enable **Start compositor automatically** and **window shadows** as desired, and use the **Edit configuration** button for providers with templates (Picom/Compton). Window Maker will create the per-user configuration automatically for those template-backed providers when you select the compositor or launch the editor, and the editor button honours `$VISUAL`, `$EDITOR`, or falls back to `xdg-open` (installable from `xdg-utils`) so the file opens in your system's default text editor. The package list above includes Mesa's GLX components so Picom/Compton presets can use the `glx` backend immediately.

## Ubuntu 25.10 (Oracular Oriole)

Ubuntu 25.10 pulls from the same Debian Trixie package set, so the installation
steps mirror Debian's closely.

### 1. Install the development dependencies

```sh
sudo apt install \
  autoconf automake autotools-dev libtool pkg-config git gettext \
  libx11-dev libxext-dev libxrandr-dev libxinerama-dev libxcursor-dev \
  libxrender-dev libxpm-dev libxmu-dev libsm-dev libice-dev \
  libfreetype6-dev libfontconfig1-dev libxft-dev \
  libpng-dev libjpeg-dev libtiff-dev libgif-dev libwebp-dev \
  libxcomposite-dev libxdamage-dev libxfixes-dev libxss-dev \
  mesa-utils libgl1-mesa-dev
```

Ubuntu does not enable `apt build-dep` for upstream Git trees by default, so the
explicit package list above is the quickest way to match the Debian build.

### 2. Fetch, configure, and build

The remaining steps are identical to Debian:

```sh
git clone https://github.com/window-maker/wmaker.git
cd wmaker
./autogen.sh
./configure --prefix=/usr/local
make -j"$(nproc)"
sudo make install
```

### 3. Keep runtime defaults in sync

After installing new builds on Ubuntu systems that previously had the distro
packages, run the per-user setup helper once so the defaults match the new
binary:

```sh
wmaker.inst
```

This refreshes the GNUstep defaults tree under `~/GNUstep/Defaults`. See
[`doc/RUNNING_WMAKER.md`](RUNNING_WMAKER.md) for more launch and troubleshooting
advice once the build is installed.

### 4. (Optional) Enable compositor integration

Ubuntu repositories provide several compositors. Install them alongside Window Maker so the Animations & Effects panel can start your selected compositor automatically:

```sh
sudo apt install picom compton xcompmgr compiz
./configure --prefix=/usr/local --with-compositor=<picom|compton|xcompmgr|compiz|none>
```

After installation, launch WPrefs, switch to **Animations & Effects**, choose your preferred backend (Picom/Compton/Xcompmgr/Compiz), enable **Start compositor automatically** if you want Window Maker
to launch the selected compositor on login, and toggle **window shadows** to preview the blended minimize/restore paths. The configuration editor in that panel updates template-backed configs (for example `~/.config/picom/picom.conf` or `~/.config/compton.conf`) the first time you open it, and the Mesa packages installed earlier provide the GLX backend Picom/Compton expect.

## Verifying the result

Both distributions place the new binaries under `/usr/local/bin` and the shared
files under `/usr/local/share/WindowMaker`. You can confirm that the build has
X11 and image support wired in by running:

```sh
wmaker --version
```

The banner should list `XPM`, `PNG`, `JPEG`, `TIFF`, `GIF`, and `WebP` support if
the packages above were installed. Use `make check` before installing if you
want the in-tree self tests to confirm the build on your machine.
