// Power-law wind profile.
//
// speed(n, z_r, v_r, altitude) = v_r * (altitude / z_r)^(1/n).
// At/below ground the profile must stay finite: pow(negative, fractional) is NaN.
#include <gtest/gtest.h>
#include <cmath>
#include "environment/wind.hpp"

using trochia::environment::wind::speed;

TEST(Wind, ReferenceHeight) {
	// at altitude == reference height the speed equals the reference speed
	EXPECT_NEAR(speed(6.0, 2.0, 5.0, 2.0), 5.0, 1e-9);
}

TEST(Wind, KnownValue) {
	// (128/2)^(1/6) = 64^(1/6) = 2  ->  5 * 2 = 10
	EXPECT_NEAR(speed(6.0, 2.0, 5.0, 128.0), 10.0, 1e-9);
}

TEST(Wind, GroundZero) {
	EXPECT_NEAR(speed(6.0, 2.0, 5.0, 0.0), 0.0, 1e-9);
}

// below ground (e.g. the final descent step) the wind must not become NaN
TEST(Wind, NegativeAltitudeFinite) {
	const double v = speed(6.0, 2.0, 5.0, -1.0);
	EXPECT_FALSE(std::isnan(v));
	EXPECT_NEAR(v, 0.0, 1e-12);
}
