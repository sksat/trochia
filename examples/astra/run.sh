#!/usr/bin/env bash
# Fetch the motor, run the validation + hazard/abort sims, sweep Cd, make plots.
set -euo pipefail
cd "$(dirname "$0")"

BIN=../../build/bin/trochia
[ -f Cesaroni_4263L1350-P.eng ] || ./fetch-engine.sh

rm -rf output output-validation

# 1) validation: reconstruct the actual flight (still air, as-flown descent)
"$BIN" config-validation.toml >/dev/null

# 2) hazard zone + abort: wind sweep x contingency scenarios
"$BIN" config.toml >/dev/null

# 3) Cd sensitivity: sweep the constant Cd in still air, record apogee
echo "# Cd apogee[m] t_apogee[s]" > cd-apogee.dat
for cd in 0.45 0.50 0.55 0.58 0.65 0.75 0.85; do
	# POSIX sed (portable across GNU/BSD): set Cd, redirect output dir
	sed -e "s/^[[:space:]]*Cd[[:space:]]*=.*/Cd = $cd/" \
	    -e 's#output-validation#.cd-out#' config-validation.toml > .cd-sweep.toml
	rm -rf .cd-out
	"$BIN" .cd-sweep.toml >/dev/null
	f=$(find .cd-out -name pos.dat | head -1)
	awk -v cd="$cd" 'NR>1{a=$3; if(a>mx){mx=a;mt=$1}} END{printf "%s %.1f %.1f\n",cd,mx,mt}' "$f" >> cd-apogee.dat
done
rm -rf .cd-out .cd-sweep.toml

# 4) plots
gnuplot validation.gp        # -> validation.png      (vs measured)
gnuplot cd-sensitivity.gp    # -> cd-sensitivity.png  (representative Cd)
gnuplot footprints.gp        # -> footprints.png      (hazard zone, East-North)
uv run footprints-map.py     # -> footprints-map.png  (hazard zone on OSM map; needs network)

echo "wrote validation.png, cd-sensitivity.png, footprints.png and footprints-map.png"
