#!/usr/bin/env bash
set -euo pipefail

export QT_QPA_PLATFORM=${QT_QPA_PLATFORM:-offscreen}
ctest --test-dir build --output-on-failure
