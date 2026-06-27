# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

trochia is a C++20 rocket flight simulator.

## How to work here

- **Get a second opinion.** For non-trivial design decisions and tricky debugging, consult the **smart-friend** skill — a knowledgeable second opinion. Bring it the problem and the options before committing to an approach, decide with it, then implement.
- **Test-driven.** Every fix/feature is test-first: write a failing test (red), then make it pass (green). Don't just assert "it runs" — assert the quantitative physics invariant (terminal velocity, bounded angle of attack, apogee, etc.), and lean on the sanitizer build for UB. Prefer tests that fail on the bug and pass on the fix.
- **Verify before claiming.** Run CI / tests / the sanitizer and read the output before saying something works; don't write comments asserting behavior you haven't observed.
- **Fine-grained PRs to `master`**, one concern each (don't mix build/CI changes with source changes). Merges are **merge-commit only** (squash and rebase are disabled on the repo). PR titles in English, bodies in Japanese, with a real explanation (symptom → root cause → fix → evidence). To illustrate a PR's effect, prefer citing an image in the PR description over committing a throwaway plot.
- **Plot with gnuplot; manage Python deps with uv** (not matplotlib / not bare pip).

## Non-obvious pitfalls (durable)

- **Eigen + `auto`:** never bind an Eigen product/expression to `auto` when an operand is a temporary — it keeps a lazy expression template referencing freed memory (use-after-scope). Use the concrete type or `.eval()`.
- **Quaternion ordering:** `math::quat2vec`/`vec2quat` are **scalar-first** `[w,x,y,z]`, but Eigen's `coeffs()` is scalar-last `[x,y,z,w]`. Mixing them silently corrupts attitude.
- **TOML number types:** wind `ground.dir`/`speed` *arrays* must be float literals (`[0.0, 15.0]`, not `[0, 15]`) or toml11 aborts; scalar fields accept int or float via the `as_number` helper.

## Orientation

All the flight physics is in one place: `do_step()` in `src/simulation.cpp`. It runs once per timestep (forces → moments → rail constraint → recovery), and the generic RK4 in `src/solver.hpp` integrates the `Object<Frame>` state from `src/object.hpp`. The rest of `src/` is header-only support (coordinate frames, atmosphere/gravity/wind, the rocket/engine models). Start from `do_step()` for anything dynamics-related.

Tests live in `test/` and run without the main build's heavy dependencies — `./test/run_tests.sh` is the fast path (system googletest + Eigen). `config-example.toml` documents the input schema; `examples/` has runnable, plotted cases.
