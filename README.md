# Trochia

[![Build Status on GitHub Actions](https://github.com/sksat/trochia/workflows/build/badge.svg)](https://github.com/sksat/trochia/actions)
[![release](https://img.shields.io/github/v/release/sksat/trochia?style=flat-square)](https://github.com/sksat/trochia/releases)
[![issues](https://img.shields.io/github/issues/sksat/trochia?color=red&style=flat-square)](https://github.com/sksat/trochia/issues)
[![license](https://img.shields.io/github/license/sksat/trochia?style=flat-square)](./LICENSE)

## trochia: trajectory of rocket and ground-hit-point calculator

Trochia is a rocket flight simulator written by modern C++.


## Install

There are pre-built binaries of latest release for some platform.

- [Linux](https://github.com/sksat/trochia/releases/latest/download/trochia-linux.zip)
- [Mac OS](https://github.com/sksat/trochia/releases/latest/download/trochia-mac.zip)
- [Windows](https://github.com/sksat/trochia/releases/latest/download/trochia-windows.zip)

Please see [Release Page](https://github.com/sksat/trochia/releases) for details.

### Build from Source

```sh
$ git clone https://github.com/sksat/trochia && cd trochia
$ mkdir build && cd build
$ cmake ..
$ make
```

## Usage

Trochia is run from the command line.
Please open your terminal and go to your project directory.

```sh
$ vim config.toml # write configuration by TOML
$ trochia
```

## Examples

Runnable use cases live in [`examples/`](examples/) (one directory each, with
config, a gnuplot script and a committed result plot). For example, the
[landing-dispersion](examples/landing-dispersion/) example sweeps wind speed ×
direction and plots the ground-hit points:

![landing dispersion](examples/landing-dispersion/dispersion.png)

## Python tooling (uv)

The simulator is C++, but a few helper scripts are written in Python. Their
dependencies are managed with [uv](https://docs.astral.sh/uv/) (pinned in
`pyproject.toml` / `uv.lock`).

```sh
$ uv sync                                  # create .venv from the lockfile
$ uv run convert-ghp.py output/85/ghp.csv  # ground-hit-point -> lat/lon for view-ghp.html
```

`convert-ghp.py` converts the simulator's `ghp.csv` output into geographic
coordinates (writing `ghp-output.js`) so the landing dispersion can be overlaid
on the map in `view-ghp.html`.

## Author

GitHub: [sksat](https://github.com/sksat)

Twitter: [@sksat\_tty](https://twitter.com/sksat_tty)

## License

See [LICENSE](./LICENSE)
