#!/usr/bin/env bash
# Download the Estes A8 motor thrust curve (RASP .eng) from ThrustCurve.org.
# (trochia reads the ';'-commented RASP file directly.)
set -euo pipefail
cd "$(dirname "$0")"
URL="https://www.thrustcurve.org/simfiles/5f4294d20002e90000000897/download/data.eng"
echo "downloading A8.eng from ThrustCurve.org ..."
curl -fsSL "$URL" -o A8.eng
echo "saved to $(pwd)/A8.eng"
