#!/usr/bin/env bash
# Run every scenario x wind direction and make the validation + footprint plots.
set -euo pipefail
cd "$(dirname "$0")"
[ -f N2501.eng ] || ./fetch-engine.sh
rm -rf output output-dualdeploy
../../build/bin/trochia config.toml            >/dev/null  # single main + scenarios
../../build/bin/trochia config-dualdeploy.toml >/dev/null  # dual deployment
gnuplot trajectory.gp          # validation vs measured telemetry
gnuplot footprints.gp          # contingency zones (East-North)
uv run footprints-map.py       # contingency zones on the OSM map (needs network)
gnuplot recovery-descent.gp    # single-main vs dual-deploy descent (mechanism)
uv run recovery-map.py         # single-main vs dual-deploy recovery zone (outcome)
echo "wrote trajectory.png, footprints.png, footprints-map.png, recovery-descent.png and recovery-map.png"
