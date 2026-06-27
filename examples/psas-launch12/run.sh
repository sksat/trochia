#!/usr/bin/env bash
# Run every scenario x wind direction and make the validation + footprint plots.
set -euo pipefail
cd "$(dirname "$0")"
[ -f N2501.eng ] || ./fetch-engine.sh
rm -rf output
../../build/bin/trochia config.toml >/dev/null
gnuplot trajectory.gp          # validation vs measured telemetry
gnuplot footprints.gp          # contingency zones (East-North)
uv run footprints-map.py       # contingency zones on the OSM map (needs network)
echo "wrote trajectory.png, footprints.png and footprints-map.png"
