#!/usr/bin/env bash
set -euo pipefail
KDIR="${KDIR:-/lib/modules/$(uname -r)/build}"
make -C "$KDIR" M="$(pwd)" modules
