#!/usr/bin/env python
"""Compare the nominal recovery zone for a single main vs dual deployment.

Reads the nominal landing footprints from the single-main run (output/nominal)
and the dual-deploy run (output-dualdeploy) and draws both on the launch-site map.

Run via uv after both trochia runs:  uv run recovery-map.py  # -> recovery-map.png
"""
import csv
import os

import pymap3d as pm
from PIL import ImageDraw, ImageFont
from staticmap import StaticMap, Line, CircleMarker

LAUNCH_LAT, LAUNCH_LON, LAUNCH_ALT = 43.7961, -120.6518, 1390.0

ZONES = [
    ("output/nominal/89/ghp.csv",      "single main (~10 km)",   "#cc0000"),
    ("output-dualdeploy/89/ghp.csv",   "dual deploy (~3 km)",    "#31688e"),
]


def enu_to_lonlat(e, n):
    x, y, z = pm.enu2ecef(e, n, 0.0, LAUNCH_LAT, LAUNCH_LON, LAUNCH_ALT)
    lat, lon, _ = pm.ecef2geodetic(x, y, z)
    return lon, lat


def loop(path):
    pts = []
    with open(path) as f:
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
    for path, _, color in ZONES:
        m.add_line(Line(loop(path), color, 3))
    m.add_marker(CircleMarker(enu_to_lonlat(0.0, 0.0), "black", 13))
    m.add_marker(CircleMarker(enu_to_lonlat(0.0, 0.0), "white", 7))

    img = m.render().convert("RGB")
    d = ImageDraw.Draw(img, "RGBA")
    font, title = find_font(15), find_font(16)
    w, h = 230, 24 * (len(ZONES) + 1) + 34
    d.rectangle([10, 10, 10 + w, 10 + h], fill=(255, 255, 255, 225), outline=(0, 0, 0, 255))
    d.text((20, 18), "nominal recovery zone", font=title, fill=(0, 0, 0))
    for i, (_, label, color) in enumerate(ZONES):
        y = 44 + i * 24
        d.line([(20, y + 8), (50, y + 8)], fill=color, width=4)
        d.text((58, y), label, font=font, fill=(0, 0, 0))
    y = 44 + len(ZONES) * 24
    d.ellipse([30, y + 2, 40, y + 12], fill="black")
    d.text((58, y), "launch (Brothers, OR)", font=font, fill=(0, 0, 0))

    img.save("recovery-map.png")
    print("saved recovery-map.png")


if __name__ == "__main__":
    main()
