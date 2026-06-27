#!/usr/bin/env bash
# Run the same flight with and without a parachute and plot the comparison.
set -euo pipefail
cd "$(dirname "$0")"

[ -f ../20191020_01.eng ] || ../fetch-engine.sh
BIN=../../build/bin/trochia

rm -rf out-chute out-ballistic
"$BIN" config.toml           >/dev/null && mv output out-chute
"$BIN" config-ballistic.toml >/dev/null && mv output out-ballistic

gnuplot plot.gp
gnuplot anim.gp
echo "wrote comparison.png and descent.gif"
