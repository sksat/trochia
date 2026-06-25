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

To overlay the dispersion on a real map, convert the ground-hit points to
lat/lon with the Python tool (see the repo README's *Python tooling* section):

```sh
uv run convert-ghp.py examples/landing-dispersion/output/85/ghp.csv   # from the repo root
```
