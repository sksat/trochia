// USSA1976 geopotential-height conversion.
//
// H = r0 * Z / (r0 + Z): geopotential height H is slightly *smaller* than the
// geometric height Z. The code used (r0 - Z), which is the wrong sign.
#include <gtest/gtest.h>
#include "environment/earth/geodesy.hpp"

using trochia::environment::earth::geodesy::potential_height;

static constexpr double r0 = 6356766.0; // effective earth radius used by the model

TEST(Geodesy, ZeroAtZero) {
	EXPECT_NEAR(potential_height(0.0), 0.0, 1e-9);
}

// geopotential height is always slightly smaller than geometric height
TEST(Geodesy, LessThanGeometric) {
	for (const double z : {100.0, 1000.0, 10000.0, 50000.0})
		EXPECT_LT(potential_height(z), z) << "z=" << z;
}

TEST(Geodesy, KnownValue10km) {
	const double z = 10000.0;
	const double expected = r0 * z / (r0 + z);   // ~= 9984.29 m
	EXPECT_NEAR(potential_height(z), expected, 1e-6);
}
