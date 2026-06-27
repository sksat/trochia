// U.S. Standard Atmosphere 1976: temperature / pressure / density as a function
// of geopotential height. Pinned against the published USSA1976 table at every
// layer base, plus the properties that the old troposphere-only model lacked:
// no NaN above ~44 km, and a finite, monotonically decreasing density all the
// way up past where a sounding rocket flies.
#include <gtest/gtest.h>
#include <cmath>
#include "environment/air.hpp"

using namespace trochia;
using environment::air::temperature;
using environment::air::pressure;
using environment::air::density;

namespace {
	// geopotential height [m], T [K], P [Pa], rho [kg/m^3] (USSA1976 table)
	struct Ref { double H, T, P, rho; };
	const Ref TABLE[] = {
		{     0.0, 288.150, 101325.0,   1.225000   },
		{ 11000.0, 216.650,  22632.0,   0.3639180  },
		{ 20000.0, 216.650,   5474.88,  0.08803491 },
		{ 32000.0, 228.650,    868.018, 0.01322500 },
		{ 47000.0, 270.650,    110.906, 0.001427534},
		{ 51000.0, 270.650,     66.9388,0.0008616040},
		{ 71000.0, 214.650,      3.95642,6.421105e-5},
		{ 84852.0, 186.946,      0.37338,6.957876e-6},
	};
}

TEST(USSA1976, MatchesPublishedTableAtLayerBases) {
	for (const auto &r : TABLE) {
		EXPECT_NEAR(temperature(r.H), r.T, 0.05) << "T at H=" << r.H;
		EXPECT_NEAR(pressure(r.H), r.P, r.P * 0.001) << "P at H=" << r.H;
		EXPECT_NEAR(density(r.H),  r.rho, r.rho * 0.001) << "rho at H=" << r.H;
	}
}

// The layered model must be continuous across every layer boundary: pressure
// and temperature just below a base must match just above it (the base-chaining
// would show a step here if a layer's formula or base value were wrong).
TEST(USSA1976, ContinuousAcrossLayerBoundaries) {
	const double BASES[] = {11000, 20000, 32000, 47000, 51000, 71000, 84852};
	for (double Hb : BASES) {
		const double below_P = pressure(Hb - 0.01), above_P = pressure(Hb + 0.01);
		const double below_T = temperature(Hb - 0.01), above_T = temperature(Hb + 0.01);
		EXPECT_NEAR(below_P, above_P, above_P * 1e-5) << "P jump at H=" << Hb;
		EXPECT_NEAR(below_T, above_T, 1e-3)           << "T jump at H=" << Hb;
	}
}

// Mid-layer points exercise the per-layer barometric formula, not just the
// base-to-base continuity. Reference values come from an INDEPENDENT USSA1976
// implementation (Python `ambiance`), evaluated at these exact geopotential
// heights -- so this cross-checks the formula against a second source, not just
// our own table. (Cross-checked across 0-80 km to < 0.001 % relative.)
TEST(USSA1976, MatchesIndependentImplMidLayer) {
	const Ref MID[] = {
		{  5000.0, 255.650, 54019.9,    0.736116    },  // layer 0 (troposphere)
		{ 25000.0, 221.650,  2511.01,   0.0394657   },  // layer 2 (+1.0 K/km)
		{ 40000.0, 251.050,   277.52,   0.00385099  },  // layer 3 (+2.8 K/km)
		{ 60000.0, 245.450,    20.3141, 0.000288319 },  // layer 5 (-2.8 K/km)
		{ 70000.0, 217.450,     4.63418,7.42423e-5  },  // layer 6 (-2.0 K/km)
	};
	for (const auto &r : MID) {
		EXPECT_NEAR(temperature(r.H), r.T, 0.05) << "T at H=" << r.H;
		EXPECT_NEAR(pressure(r.H), r.P, r.P * 0.001) << "P at H=" << r.H;
		EXPECT_NEAR(density(r.H),  r.rho, r.rho * 0.001) << "rho at H=" << r.H;
	}
}

// The old model went NaN above ~44 km (T <= 0 K -> pow of a negative base).
// Every altitude a sounding rocket reaches must give a finite, positive density.
TEST(USSA1976, FiniteEverywhereNoNaN) {
	for (double H = 0.0; H <= 120000.0; H += 1000.0) {
		const double rho = density(H);
		const double p   = pressure(H);
		const double T   = temperature(H);
		EXPECT_TRUE(std::isfinite(rho) && rho > 0.0) << "rho at H=" << H;
		EXPECT_TRUE(std::isfinite(p) && p > 0.0)     << "P at H=" << H;
		EXPECT_TRUE(std::isfinite(T) && T > 0.0)     << "T at H=" << H;
	}
}

// Density must decrease monotonically with height (the troposphere model did
// not, near the layer where T crossed zero).
TEST(USSA1976, DensityMonotonicDecrease) {
	double prev = density(0.0);
	for (double H = 1000.0; H <= 120000.0; H += 1000.0) {
		const double rho = density(H);
		EXPECT_LT(rho, prev) << "rho not decreasing at H=" << H;
		prev = rho;
	}
}

// Above the 86 km (H=84852 m) table top we extrapolate isothermally; density
// keeps falling smoothly toward zero, well past MOMO's ~113 km apogee.
TEST(USSA1976, AboveTableTopExtrapolatesSmall) {
	const double top = density(84852.0);
	const double high = density(111000.0);   // ~113 km geometric
	EXPECT_GT(high, 0.0);
	EXPECT_LT(high, top);
	EXPECT_LT(high, 1e-4);                    // genuinely thin air up there
	EXPECT_DOUBLE_EQ(temperature(111000.0), temperature(90000.0)); // isothermal
}

// Below sea level is clamped to sea-level conditions (no negative-height blowup).
TEST(USSA1976, BelowSeaLevelClamped) {
	EXPECT_NEAR(density(-100.0), density(0.0), 1e-12);
	EXPECT_NEAR(temperature(-100.0), 288.150, 1e-9);
}
