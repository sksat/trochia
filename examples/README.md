# Examples

Runnable example simulations for trochia, one directory per use case.

Each example is self-contained: its own `config.toml`, a gnuplot script, a
committed result plot, and a `README.md`. The motor thrust curve is **not**
committed — fetch it once from the gist:

```sh
$ ./fetch-engine.sh        # downloads 20191020_01.eng (LARKSPUR-XP.300) here
```

You also need the `trochia` binary — build it from the repo root (see the top
[README](../README.md)); it ends up at `build/bin/trochia`. trochia always reads
`config.toml` from the current directory and writes results under `output/`, so
run each example from its own directory.

## Use cases

| example | use case |
|---|---|
| [single-trajectory](single-trajectory/)   | one trajectory for a single launch condition (apogee / downrange / GHP) |
| [landing-dispersion](landing-dispersion/) | landing dispersion (落下分散) over a wind speed × direction sweep |

See each directory's `README.md` for how to run it and the resulting plot.
