#!/usr/bin/env python
"""Overlay the contingency landing footprints on the real launch site map.

Reads each scenario's ground-hit points (output/<scenario>/89/ghp.csv), converts
the local ENU coordinates to lat/lon (pymap3d), and draws one footprint per
scenario on OpenStreetMap tiles (staticmap).

Run via uv after trochia:   uv run footprints-map.py   # -> footprints-map.png
"""
import csv
import os

import pymap3d as pm
from PIL import ImageDraw, ImageFont
from staticmap import StaticMap, Line, CircleMarker

# PSAS launch site: Brothers, Oregon
LAUNCH_LAT, LAUNCH_LON, LAUNCH_ALT = 43.7961, -120.6518, 1390.0

SCENARIOS = [
    ("nominal",       "nominal (chute)",            "#31688e"),
    ("chute-failure", "chute failure (ballistic)",  "#cc0000"),
    ("cato",          "CATO at 3 s",                "#4ac16d"),
]


def enu_to_lonlat(e, n):
    x, y, z = pm.enu2ecef(e, n, 0.0, LAUNCH_LAT, LAUNCH_LON, LAUNCH_ALT)
    lat, lon, _ = pm.ecef2geodetic(x, y, z)
    return lon, lat


def loop(scenario):
    pts = []
    with open(os.path.join("output", scenario, "89", "ghp.csv")) as f:
        for row in csv.DictReader(f, skipinitialspace=True):
            pts.append(enu_to_lonlat(float(row["ghp_e"]), float(row["ghp_n"])))
    return pts


def find_font(size):
    for p in ("/usr/share/fonts/TTF/Roboto-Regular.ttf",
              "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"):
        try:
            return ImageFont.truetype(p, size)
        except OSError:
            continue
    return ImageFont.load_default()


def main():
    m = StaticMap(900, 900, url_template="https://tile.openstreetmap.org/{z}/{x}/{y}.png")
    for name, _, color in SCENARIOS:
        m.add_line(Line(loop(name), color, 3))
    m.add_marker(CircleMarker(enu_to_lonlat(0.0, 0.0), "black", 13))
    m.add_marker(CircleMarker(enu_to_lonlat(0.0, 0.0), "white", 7))

    img = m.render().convert("RGB")
    d = ImageDraw.Draw(img, "RGBA")
    font, title = find_font(15), find_font(16)
    w, h = 230, 24 * (len(SCENARIOS) + 1) + 34
    d.rectangle([10, 10, 10 + w, 10 + h], fill=(255, 255, 255, 225), outline=(0, 0, 0, 255))
    d.text((20, 18), "contingency footprint", font=title, fill=(0, 0, 0))
    for i, (_, label, color) in enumerate(SCENARIOS):
        y = 44 + i * 24
        d.line([(20, y + 8), (50, y + 8)], fill=color, width=4)
        d.text((58, y), label, font=font, fill=(0, 0, 0))
    y = 44 + len(SCENARIOS) * 24
    d.ellipse([30, y + 2, 40, y + 12], fill="black")
    d.text((58, y), "launch (Brothers, OR)", font=font, fill=(0, 0, 0))

    img.save("footprints-map.png")
    print("saved footprints-map.png")


if __name__ == "__main__":
    main()
