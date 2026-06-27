#!/usr/bin/env bash
# Download the Cesaroni 4263L1350-P motor thrust curve (RASP .eng).
# This is the exact file used by RocketPy's "Astra" model, so the motor matches
# the rocket parameters in config.toml.
set -euo pipefail
cd "$(dirname "$0")"
URL="https://raw.githubusercontent.com/RocketPy-Team/RocketPy/master/data/motors/cesaroni/Cesaroni_4263L1350-P.eng"
echo "downloading Cesaroni_4263L1350-P.eng ..."
curl -fsSL "$URL" -o Cesaroni_4263L1350-P.eng
echo "saved to $(pwd)/Cesaroni_4263L1350-P.eng"
