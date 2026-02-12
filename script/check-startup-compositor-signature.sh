#!/bin/sh
set -eu

ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
FILE="$ROOT_DIR/src/startup.c"

fail() {
  echo "check-startup-compositor-signature: $*" >&2
  exit 1
}

[ -f "$FILE" ] || fail "missing src/startup.c"

grep -q "^static Bool ensure_backend_config(const char \*template_name, const char \*path);" "$FILE" \
  || fail "prototype for ensure_backend_config is not template_name/path"

grep -q "^static Bool ensure_backend_config(const char \*template_name, const char \*path)$" "$FILE" \
  || fail "definition for ensure_backend_config is not template_name/path"

grep -q "int ch;b" "$FILE" && fail "found malformed token sequence: int ch;b"
grep -q "if (!backend || !path || !\*path)" "$FILE" && fail "found stale backend pointer guard"
grep -q "template_name = \"picom.conf\";" "$FILE" && fail "found stale template_name assignment"
grep -q "compositor_template_for_choice" "$FILE" || fail "missing compositor template mapping helper"



echo "check-startup-compositor-signature: ok"
