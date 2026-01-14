# Running Window Maker

Window Maker ships as a stand-alone X11 window manager, so starting it after
you install the binaries involves wiring it into the session scripts your
system already uses. This guide walks through the common entry points and how
they interact with the GNUstep configuration that Window Maker maintains.

## 1. Run the post-install setup

Window Maker stores its per-user defaults under `~/GNUstep/Defaults` and
expects a minimal directory layout before it can start. After installing the
packages, run the helper once as the user who will log in to X11:

```sh
wmaker.inst
```

The script copies the global defaults into your home directory and prepares the
menu, style, icon, and state files that the window manager and WPrefs.app read.
You only need to rerun it if you want to reset everything back to the system
defaults. Future releases merge new options automatically, so your existing
preferences remain valid.

## 2. Start Window Maker from `startx`/`xinit`

If you launch X11 manually with `startx` or `xinit`, create an executable
`~/.xinitrc` (or `~/.xsession` on systems that read it) that ends by exec'ing
Window Maker:

```sh
#!/bin/sh
# ~/.xinitrc – start Window Maker
# launch any environment daemons you need here
exec wmaker
```

Make the script executable (`chmod +x ~/.xinitrc`) so your X session picks it
up. When you run `startx`, the script will start the daemons you listed and then
hand control to Window Maker. Use absolute paths if your installation lives
outside the default `$PATH`.

## 3. Add a desktop session entry for a display manager

Modern display managers (GDM, LightDM, SDDM, XDM) look for desktop session
entries under `/usr/share/xsessions`. Create a file such as
`/usr/share/xsessions/WindowMaker.desktop` with root privileges:

```ini
[Desktop Entry]
Name=Window Maker
Comment=NeXTSTEP-inspired window manager
Exec=wmaker
TryExec=wmaker
Type=Application
DesktopNames=WindowMaker
```

After you add the file, Window Maker appears in the session chooser at the login
screen. The `TryExec` line lets the display manager hide the entry if the binary
is missing. You can also place the file in `/usr/local/share/xsessions` if your
system searches that directory instead.

## 4. Replace an already running window manager

If you are inside another desktop and want to try Window Maker without logging
out, request ownership of the X11 screens that the current window manager is
handling:

```sh
wmaker --replace
```

This sends the standard ICCCM/NETWM message asking the active window manager to
exit and immediately starts Window Maker. Use your original environment's
session controls to switch back later.

## 5. Customize what starts with Window Maker

Once Window Maker is running, it executes
`~/GNUstep/Library/WindowMaker/autostart` on login and
`~/GNUstep/Library/WindowMaker/exitscript` right before it exits. The defaults
created by `wmaker.inst` include placeholders for these files, so you can drop
shell commands there to start panels, compositors, or status daemons alongside
Window Maker.

You can fine-tune menu contents, dock behavior, and workspace policies through
WPrefs.app. The tool writes back into the same defaults directory, so any
changes you make are automatically picked up the next time Window Maker starts.

## 6. Troubleshooting startup

* If `wmaker` exits complaining about missing defaults, rerun `wmaker.inst` to
  repopulate `~/GNUstep`.
* When a display manager ignores your new session entry, verify the `.desktop`
  file permissions and confirm that the `Exec` path is correct.
* If the session scripts report “permission denied,” double-check that
  `~/.xinitrc` (or `.xsession`) is executable.

With these pieces in place, you can rebuild your X11 session around Window Maker
and still keep the GNUstep-aware configuration tools that ship with the window
manager.
