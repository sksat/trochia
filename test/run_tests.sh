#!/usr/bin/env bash
# Lightweight test runner: compiles each test/test_*.cpp against the header-only
# parts of src/ using the system Eigen and links googletest. It deliberately
# avoids the heavy ExternalProject deps of the main build.
#
# For the CMake/ctest path instead, see test/CMakeLists.txt.
set -uo pipefail

cd "$(dirname "$0")/.."

SRC=src
EIGEN=${EIGEN_INCLUDE:-/usr/include/eigen3}
CXX=${CXX:-g++}
FLAGS=(-std=c++20 -I "$SRC" -I "$EIGEN" -Wall -Wextra -g)
GTEST=(-lgtest -lgtest_main -lpthread)

out=$(mktemp -d)
trap 'rm -rf "$out"' EXIT

fail=0
for t in test/test_*.cpp; do
	name=$(basename "$t" .cpp)
	# tests named test_sim_* exercise the simulation, so compile its source too
	extra=()
	if [[ "$name" == test_sim_* ]]; then
		extra+=(src/simulation.cpp)
	fi
	echo "=== building $name ==="
	if ! "$CXX" "${FLAGS[@]}" "$t" "${extra[@]}" -o "$out/$name" "${GTEST[@]}"; then
		echo "BUILD FAILED: $name"
		fail=1
		continue
	fi
	echo "=== running $name ==="
	if ! "$out/$name"; then
		fail=1
	fi
done

if [ "$fail" -ne 0 ]; then
	echo "SOME TESTS FAILED"
else
	echo "ALL TESTS PASSED"
fi
exit "$fail"
