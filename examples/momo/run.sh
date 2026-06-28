#!/usr/bin/env bash
# Run the MOMO vertical flight + abort envelope, the unguided tilt demo, the
# effective-thrust sweep, and make the plots.
set -euo pipefail
cd "$(dirname "$0")"

BIN=../../build/bin/trochia
[ -x "$BIN" ] || { echo "build trochia first (see repo README): $BIN not found" >&2; exit 1; }
command -v gnuplot >/dev/null || { echo "gnuplot is required" >&2; exit 1; }
rm -rf output output-tilt .sweep-out

# 1) nominal flight + abort vertical energy envelope (MOMO-1/2/3/4)
"$BIN" config.toml >/dev/null

# 2) unguided tilt-over demo (0.1 deg off vertical) -- the model's limit
"$BIN" config-tilt-demo.toml >/dev/null

# 3) effective-thrust sweep (vertical, nominal only) -> thrust-apogee.dat
echo "# thrust[kN] apogee[km] t_apogee[s]" > thrust-apogee.dat
sed -e '/^\[\[scenario\]\]/,$d' \
    -e 's/engine = "momo.eng"/engine = ".sweep.eng"/' \
    -e 's/output.dir = "output"/output.dir = ".sweep-out"/' config.toml > .sweep.toml
for th in 12.0 12.5 13.0 13.5 13.9 14.5 16.0; do
	thN=$(awk -v k="$th" 'BEGIN{printf "%.1f", k*1000}')
	cat > .sweep.eng <<EOF
MOMO-sweep 500 9900 P 820.0 820.0 IST
0.0 0.0
1.0 $thN
120.0 $thN
121.0 0.0
EOF
	rm -rf .sweep-out
	"$BIN" .sweep.toml >/dev/null
	f=".sweep-out/90/0.001/0/pos.dat"   # deterministic path (no find|head)
	[ -s "$f" ] || { echo "sweep run produced no output: $f" >&2; exit 1; }
	awk -v k="$th" 'NR>1{u=$3; if(u>mx){mx=u;mt=$1}} END{printf "%s %.1f %.0f\n",k,mx/1000,mt}' "$f" >> thrust-apogee.dat
done
rm -rf .sweep-out .sweep.toml .sweep.eng

# 4) plots
gnuplot altitude.gp        # -> altitude.png       (nominal + abort envelope vs real apogees)
gnuplot thrust-sweep.gp    # -> thrust-sweep.png   (effective-thrust calibration)
gnuplot unguided-limit.gp  # -> unguided-limit.png (why MOMO needs TVC)
echo "wrote altitude.png, thrust-sweep.png and unguided-limit.png"
