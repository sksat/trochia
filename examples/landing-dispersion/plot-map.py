#!/usr/bin/env python
"""Render the landing dispersion on an OpenStreetMap basemap.

Reads the simulator's ground-hit-point output (ghp.csv), converts the local
ENU coordinates to latitude/longitude (pymap3d), and draws one dispersion loop
per wind speed over OSM tiles (staticmap), with a legend.

Run via uv from this directory (after trochia has produced output/):
    uv run plot-map.py                 # -> dispersion-map.png
    uv run plot-map.py output/72/ghp.csv my-map.png
"""
import csv
import sys

import pymap3d as pm
from PIL import Image, ImageDraw, ImageFont
from staticmap import StaticMap, Line, CircleMarker

# launch site: Izu Oshima / Toshiki (matches convert-ghp.py)
LAUNCH_LAT, LAUNCH_LON, LAUNCH_ALT = 34.679730, 139.438373, 39.4422

# viridis-ish colours, one per wind speed (m/s)
COLORS = ["#440154", "#46327e", "#365c8d", "#277f8e", "#1fa187", "#4ac16d", "#fde725"]

GHP = sys.argv[1] if len(sys.argv) > 1 else "output/72/ghp.csv"
OUT = sys.argv[2] if len(sys.argv) > 2 else "dispersion-map.png"


def enu_to_lonlat(e, n):
    x, y, z = pm.enu2ecef(e, n, 0.0, LAUNCH_LAT, LAUNCH_LON, LAUNCH_ALT)
    lat, lon, _ = pm.ecef2geodetic(x, y, z)
    return lon, lat  # staticmap expects (lon, lat)


def load_loops(path):
    loops = {}
    with open(path) as f:
        for row in csv.DictReader(f, skipinitialspace=True):
            ws = float(row["wspeed"])
            loops.setdefault(ws, []).append(
                enu_to_lonlat(float(row["ghp_e"]), float(row["ghp_n"])))
    return dict(sorted(loops.items()))


def find_font(size):
    for path in (
        "/usr/share/fonts/TTF/Roboto-Regular.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/liberation/LiberationSans-Regular.ttf",
        "/Library/Fonts/Arial.ttf",
    ):
        try:
            return ImageFont.truetype(path, size)
        except OSError:
            continue
    return ImageFont.load_default()


def draw_legend(img, speeds):
    d = ImageDraw.Draw(img, "RGBA")
    font = find_font(15)
    title = find_font(16)
    rows = len(speeds) + 1
    w, h = 150, 24 * rows + 34
    x0, y0 = img.width - w - 12, 12
    d.rectangle([x0, y0, x0 + w, y0 + h], fill=(255, 255, 255, 220),
                outline=(0, 0, 0, 255))
    d.text((x0 + 10, y0 + 8), "wind speed", font=title, fill=(0, 0, 0))
    for i, ws in enumerate(speeds):
        y = y0 + 34 + i * 24
        d.line([(x0 + 10, y + 8), (x0 + 40, y + 8)], fill=COLORS[i % len(COLORS)], width=4)
        d.text((x0 + 48, y), f"{ws:.0f} m/s", font=font, fill=(0, 0, 0))
    y = y0 + 34 + len(speeds) * 24
    d.ellipse([x0 + 20, y + 2, x0 + 30, y + 12], fill="red")
    d.text((x0 + 48, y), "launch", font=font, fill=(0, 0, 0))


def main():
    loops = load_loops(GHP)
    m = StaticMap(900, 900, url_template="https://tile.openstreetmap.org/{z}/{x}/{y}.png")
    for i, (ws, pts) in enumerate(loops.items()):
        m.add_line(Line(pts, COLORS[i % len(COLORS)], 3))  # cycle, never truncate
    m.add_marker(CircleMarker(enu_to_lonlat(0.0, 0.0), "red", 13))
    m.add_marker(CircleMarker(enu_to_lonlat(0.0, 0.0), "white", 7))

    img = m.render().convert("RGB")
    draw_legend(img, list(loops.keys()))
    img.save(OUT)
    print(f"saved {OUT} ({img.width}x{img.height}) from {GHP}")


if __name__ == "__main__":
    main()
