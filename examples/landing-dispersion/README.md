# Landing dispersion (wind speed × direction sweep)

**Use case:** estimate the landing dispersion (落下分散) by sweeping wind speed
and direction — the spread of ground-hit points used for range-safety planning.

Elevation 72 deg, wind speed 1-7 m/s x direction every 15 deg (0..360) = 175 runs.
The launcher is tilted downrange (azimuth ~150 deg, toward the open sea) so the
whole dispersion clears the launch site — see the map below.

## Run

```sh
../fetch-engine.sh            # once: downloads ../20191020_01.eng
../../build/bin/trochia        # 175 runs -> output/72/ghp.csv (+ per-case *.dat)
gnuplot plot.gp                # -> dispersion.png
```

## Result

One smooth closed loop per wind speed (24 directions, 15 deg apart); apogee
~279-344 m, dropping with wind as the rocket weathercocks. Because the launcher
is tilted ~18 deg off vertical toward the open sea, every loop — even the 7 m/s
one — lands offshore, ~300-450 m downrange of the pad. A near-vertical launch
would instead drop the dispersion onto the launch site (dangerous).

![landing dispersion](dispersion.png)

### On a map

`plot-map.py` draws the same dispersion on an OpenStreetMap basemap (the launch
site is Izu Oshima / Toshiki), so you can see which landing points fall on land
versus in the sea — the actual range-safety question:

```sh
uv run plot-map.py            # -> dispersion-map.png  (needs network for the tiles)
```

![landing dispersion on a map](dispersion-map.png)

For an interactive version, `view-ghp.html` (at the repo root) overlays the same
points on a Leaflet map. Generate its data **from the repo root** (where
convert-ghp.py and view-ghp.html live), so `ghp-output.js` is written next to the
HTML:

```sh
uv run convert-ghp.py examples/landing-dispersion/output/72/ghp.csv
```
