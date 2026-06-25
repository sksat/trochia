#!/usr/bin/env bash
# Download the example motor (LARKSPUR-XP.300, a 2019-10-20 burn test) used by
# the example configs. The thrust curve is kept in a gist rather than committed
# to the repo, so fetch it here before running an example.
set -euo pipefail
cd "$(dirname "$0")"

URL="https://gist.githubusercontent.com/sksat/c4a90991af7d24c654a8bc5658aeb696/raw/20191020_01.eng"
DEST="20191020_01.eng"

echo "downloading ${DEST} from gist ..."
curl -fsSL "$URL" -o "$DEST"
echo "saved to $(pwd)/${DEST}"
