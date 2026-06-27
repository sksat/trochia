# Examples

Runnable example simulations for trochia, one directory per use case.

Each example is self-contained: its own `config.toml`, a plotting script, a
committed result plot, and a `README.md`. Motor thrust curves are **not**
committed — each example fetches its own:

```sh
$ ./fetch-engine.sh        # LARKSPUR-XP.300 (20191020_01.eng), used by
                           # single-trajectory, landing-dispersion and parachute
```

`validation-estes-viking` fetches the Estes A8 from ThrustCurve.org with its own
`fetch-engine.sh`.

You also need the `trochia` binary — build it from the repo root (see the top
[README](../README.md)); it ends up at `build/bin/trochia`. trochia always reads
`config.toml` from the current directory and writes results under `output/`, so
run each example from its own directory.

## Use cases

| example | use case |
|---|---|
| [single-trajectory](single-trajectory/)   | one trajectory for a single launch condition (apogee / downrange / GHP) |
| [landing-dispersion](landing-dispersion/) | landing dispersion (落下分散) over a wind speed × direction sweep |
| [parachute](parachute/)                   | parachute vs ballistic descent — how recovery changes the landing point |
| [validation-estes-viking](validation-estes-viking/) | accuracy check: predicted vs **measured** apogee of a real flight |

See each directory's `README.md` for how to run it and the resulting plot.
