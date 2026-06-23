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

## Author

GitHub: [sksat](https://github.com/sksat)

Twitter: [@sksat\_tty](https://twitter.com/sksat_tty)

## License

See [LICENSE](./LICENSE)
