#!/usr/bin/env python
"""Overlay the contingency landing footprints + the real GPS track on the map.

Reads each scenario's ground-hit points (output/<scenario>/84/ghp.csv), converts
the local ENU coordinates to lat/lon (pymap3d), and draws one footprint per
scenario on OpenStreetMap tiles (staticmap). The rocket's measured GPS track
(measured-track.dat, up to ~145 m AGL where the record ends) is drawn too.

Run via uv after trochia:   uv run footprints-map.py   # -> footprints-map.png
"""
import csv

import pymap3d as pm
from PIL import ImageDraw, ImageFont
from staticmap import StaticMap, Line, CircleMarker

# Launch site: Ponte de Sor, Portugal (EuRoC). First GPS fix in flight_data.
LAUNCH_LAT, LAUNCH_LON, LAUNCH_ALT = 39.3898, -8.290, 160.0

SCENARIOS = [
    ("nominal",          "nominal (dual deploy)",      "#31688e"),
    ("recovery-failure", "recovery failure (ballistic)", "#cc0000"),
    ("motor-cutoff",     "motor cutoff @ 1.5 s",       "#dd8800"),
    ("cato",             "CATO @ 1.0 s",               "#4ac16d"),
]


def enu_to_lonlat(e, n):
    x, y, z = pm.enu2ecef(e, n, 0.0, LAUNCH_LAT, LAUNCH_LON, LAUNCH_ALT)
    lat, lon, _ = pm.ecef2geodetic(x, y, z)
    return lon, lat


def loop(scenario):
    pts = []
    with open(f"output/{scenario}/84/ghp.csv") as f:
        for row in csv.DictReader(f, skipinitialspace=True):
            pts.append(enu_to_lonlat(float(row["ghp_e"]), float(row["ghp_n"])))
    return pts


def measured_track():
    pts = []
    with open("measured-track.dat") as f:
        for line in f:
            if line.startswith("#"):
                continue
            _, lat, lon, _ = line.split()
            lat, lon = float(lat), float(lon)
            if abs(lat) < 1.0 and abs(lon) < 1.0:
                continue  # GPS dropout fix (lat=lon~0); skip
            pts.append((lon, lat))
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
    m = StaticMap(950, 950, url_template="https://tile.openstreetmap.org/{z}/{x}/{y}.png")
    for name, _, color in SCENARIOS:
        m.add_line(Line(loop(name), color, 3))
    # measured GPS track (white casing + dark line), ends at ~145 m AGL
    track = measured_track()
    m.add_line(Line(track, "white", 5))
    m.add_line(Line(track, "#222222", 2))
    m.add_marker(CircleMarker(enu_to_lonlat(0.0, 0.0), "black", 13))
    m.add_marker(CircleMarker(enu_to_lonlat(0.0, 0.0), "white", 7))

    img = m.render().convert("RGB")
    d = ImageDraw.Draw(img, "RGBA")
    font, title = find_font(15), find_font(16)
    rows = SCENARIOS + [(None, "measured GPS track", "#222222")]
    w, h = 250, 24 * (len(rows) + 1) + 34
    d.rectangle([10, 10, 10 + w, 10 + h], fill=(255, 255, 255, 225), outline=(0, 0, 0, 255))
    d.text((20, 18), "contingency footprints", font=title, fill=(0, 0, 0))
    for i, (_, label, color) in enumerate(rows):
        y = 44 + i * 24
        d.line([(20, y + 8), (50, y + 8)], fill=color, width=4)
        d.text((58, y), label, font=font, fill=(0, 0, 0))
    y = 44 + len(rows) * 24
    d.ellipse([30, y + 2, 40, y + 12], fill="black")
    d.text((58, y), "launch (Ponte de Sor)", font=font, fill=(0, 0, 0))

    img.save("footprints-map.png")
    print("saved footprints-map.png")


if __name__ == "__main__":
    main()
