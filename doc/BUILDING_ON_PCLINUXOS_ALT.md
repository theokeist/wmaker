# Building Window Maker on PCLinuxOS and ALT Linux

This guide captures the package sets and build steps that have been tested on
PCLinuxOS and ALT Linux. The lists below intentionally include both required
and optional image/compositor libraries so Window Maker builds with the full
texture and icon pipeline enabled.

## Common preparation

Install the toolchain and autotools needed for Git checkouts (use `dnf` on
RPM-based PCLinuxOS spins):

```sh
sudo apt-get update
sudo apt-get install gcc make autoconf automake libtool pkgconfig git gettext
```

```sh
sudo dnf install gcc make autoconf automake libtool pkgconfig git gettext
```

If you are building from a release tarball, `autoconf`/`automake`/`libtool` are
already handled and you can skip them.

## PCLinuxOS

### 1. Install development libraries

```sh
sudo apt-get install \
  libX11-devel libXext-devel libXrandr-devel libXinerama-devel libXcursor-devel \
  libXrender-devel libXpm-devel libXmu-devel libSM-devel libICE-devel \
  freetype-devel fontconfig-devel libXft-devel \
  libpng-devel libjpeg-devel libtiff-devel giflib-devel libwebp-devel \
  libXcomposite-devel libXdamage-devel libXfixes-devel libXss-devel \
  mesa-libGL-devel mesa-utils
```

```sh
sudo dnf install \
  libX11-devel libXext-devel libXrandr-devel libXinerama-devel libXcursor-devel \
  libXrender-devel libXpm-devel libXmu-devel libSM-devel libICE-devel \
  freetype-devel fontconfig-devel libXft-devel \
  libpng-devel libjpeg-devel libtiff-devel giflib-devel libwebp-devel \
  libXcomposite-devel libXdamage-devel libXfixes-devel libXss-devel \
  mesa-libGL-devel mesa-utils
```

If any of the packages above are named differently on your mirror, search with
`apt-cache search libX11` and substitute the closest `-devel` package.

### 2. Build and install

```sh
git clone https://github.com/window-maker/wmaker.git
cd wmaker
./autogen.sh
./configure --prefix=/usr/local
make -j"$(nproc)"
sudo make install
```

### 3. (Optional) Enable compositor defaults

```sh
sudo apt-get install picom compton xcompmgr compiz
./configure --prefix=/usr/local --with-compositor=<picom|compton|xcompmgr|compiz|none>
```

```sh
sudo dnf install picom compton xcompmgr compiz
./configure --prefix=/usr/local --with-compositor=<picom|compton|xcompmgr|compiz|none>
```

After installation, open **WPrefs → Animations & Effects**, choose your preferred compositor, then enable **Start compositor automatically** and **window shadows** if you want the modern effects path. The GLX backend needs `mesa-libGL-devel` at build and `mesa-utils` at runtime. Picom and Compton also expose template-backed configuration editing from the same panel.

## ALT Linux

### 1. Install development libraries

```sh
sudo apt-get update
sudo apt-get install \
  libX11-devel libXext-devel libXrandr-devel libXinerama-devel libXcursor-devel \
  libXrender-devel libXpm-devel libXmu-devel libSM-devel libICE-devel \
  freetype-devel fontconfig-devel libXft-devel \
  libpng-devel libjpeg-devel libtiff-devel giflib-devel libwebp-devel \
  libXcomposite-devel libXdamage-devel libXfixes-devel libXss-devel \
  mesa-libGL-devel mesa-utils
```

ALT Linux repositories sometimes split `pkgconfig` as `pkg-config`. If the
installer asks, choose the `pkg-config` package that provides `/usr/bin/pkg-config`.

### 2. Build and install

```sh
git clone https://github.com/window-maker/wmaker.git
cd wmaker
./autogen.sh
./configure --prefix=/usr/local
make -j"$(nproc)"
sudo make install
```

### 3. (Optional) Enable compositor defaults

```sh
sudo apt-get install picom compton xcompmgr compiz
./configure --prefix=/usr/local --with-compositor=<picom|compton|xcompmgr|compiz|none>
```

Select your compositor in **WPrefs → Animations & Effects**, then toggle the compositor and shadow options. For Picom/Compton, the panel can initialize and open a template-backed config (for example `~/.config/picom/picom.conf` or `~/.config/compton.conf`).

## Verifying the result

After installation, confirm that the optional image libraries were detected:

```sh
wmaker --version
```

The banner should list support for XPM, PNG, JPEG, TIFF, GIF, and WebP if the
packages above were installed. For runtime setup and troubleshooting, see
[`doc/RUNNING_WMAKER.md`](RUNNING_WMAKER.md).


> You can choose a different default compositor at configure time with `--with-compositor=compton|xcompmgr|compiz|none`.
