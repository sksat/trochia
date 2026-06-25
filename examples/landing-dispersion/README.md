# Landing dispersion (wind speed × direction sweep)

**Use case:** estimate the landing dispersion (落下分散) by sweeping wind speed
and direction — the spread of ground-hit points used for range-safety planning.

Elevation 85 deg, wind speed 1-7 m/s x direction every 15 deg (0..360) = 175 runs.

## Run

```sh
../fetch-engine.sh            # once: downloads ../20191020_01.eng
../../build/bin/trochia        # 32 runs -> output/85/ghp.csv (+ per-case *.dat)
gnuplot plot.gp                # -> dispersion.png
```

## Result

One smooth closed loop per wind speed (24 directions, 15 deg apart). Dispersion
grows with wind (radius ~134 m at 1 m/s to ~237 m at 7 m/s) while apogee drops
(~360 -> ~329 m) as the rocket weathercocks into the wind.

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
uv run convert-ghp.py examples/landing-dispersion/output/85/ghp.csv
```
