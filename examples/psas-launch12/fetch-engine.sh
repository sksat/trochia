#!/usr/bin/env bash
# Download the Cesaroni N2501 motor thrust curve (RASP .eng) from ThrustCurve.org.
set -euo pipefail
cd "$(dirname "$0")"
URL="https://www.thrustcurve.org/simfiles/5f4294d20002e90000000767/download/data.eng"
echo "downloading N2501.eng from ThrustCurve.org ..."
curl -fsSL "$URL" -o N2501.eng
echo "saved to $(pwd)/N2501.eng"
