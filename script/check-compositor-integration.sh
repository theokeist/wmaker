#!/bin/sh
set -eu

ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"

fail() {
  echo "check-compositor-integration: $*" >&2
  exit 1
}

[ -f "$ROOT_DIR/src/defaults.c" ] || fail "missing src/defaults.c"
[ -f "$ROOT_DIR/src/startup.c" ] || fail "missing src/startup.c"
[ -f "$ROOT_DIR/src/dock.c" ] || fail "missing src/dock.c"
[ -f "$ROOT_DIR/src/compositor.c" ] || fail "missing src/compositor.c"
[ -f "$ROOT_DIR/configure.ac" ] || fail "missing configure.ac"
[ -f "$ROOT_DIR/WindowMaker/Compositors/compton.conf" ] || fail "missing Compton template"

grep -q '"DockOpacity"' "$ROOT_DIR/src/defaults.c" || fail "DockOpacity default key not found"
grep -q 'updateDockOpacity' "$ROOT_DIR/src/defaults.c" || fail "DockOpacity callback not wired"
grep -q 'wDockApplyOpacity' "$ROOT_DIR/src/defaults.c" || fail "Dock opacity callback does not apply runtime change"

grep -q 'wCompositorGetBackend' "$ROOT_DIR/src/startup.c" || fail "startup does not use compositor backend interface"
grep -q 'wCompositorBuildLaunchCommand' "$ROOT_DIR/src/startup.c" || fail "startup does not use compositor launch builder"

grep -q 'wDockApplyOpacity' "$ROOT_DIR/src/dock.c" || fail "dock opacity applicator missing"

grep -q 'WCOMPOSITOR_PICOM' "$ROOT_DIR/src/compositor.c" || fail "picom backend missing"
grep -q 'WCOMPOSITOR_COMPTON' "$ROOT_DIR/src/compositor.c" || fail "compton backend missing"
grep -q 'WCOMPOSITOR_XCOMPMGR' "$ROOT_DIR/src/compositor.c" || fail "xcompmgr backend missing"
grep -q 'WCOMPOSITOR_COMPIZ' "$ROOT_DIR/src/compositor.c" || fail "compiz backend missing"
grep -q 'WCOMPOSITOR_NONE' "$ROOT_DIR/src/compositor.c" || fail "none backend missing"

grep -q '"Compton"' "$ROOT_DIR/src/defaults.c" || fail "defaults missing Compton provider"
grep -q '"Xcompmgr"' "$ROOT_DIR/src/defaults.c" || fail "defaults missing Xcompmgr provider"
grep -q '"Compiz"' "$ROOT_DIR/src/defaults.c" || fail "defaults missing Compiz provider"

grep -q '\[compton\]' "$ROOT_DIR/configure.ac" || fail "configure missing compton option"
grep -q '\[xcompmgr\]' "$ROOT_DIR/configure.ac" || fail "configure missing xcompmgr option"
grep -q '\[compiz\]' "$ROOT_DIR/configure.ac" || fail "configure missing compiz option"

echo "check-compositor-integration: ok"
