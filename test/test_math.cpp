// sanity tests for the math helpers (validates the test harness itself)
#include <gtest/gtest.h>
#include <cmath>
#include "math.hpp"

namespace m = trochia::math;

TEST(Math, Deg2Rad) { EXPECT_NEAR(m::deg2rad(180.0), m::pi, 1e-12); }
TEST(Math, Rad2Deg) { EXPECT_NEAR(m::rad2deg(m::pi), 180.0, 1e-12); }
TEST(Math, Lerp)    { EXPECT_NEAR(m::lerp(0.0, 10.0, 0.25), 2.5, 1e-12); }

TEST(Math, Sqrt) {
	EXPECT_NEAR(m::sqrt(2.0), std::sqrt(2.0), 1e-9);
	EXPECT_NEAR(m::sqrt(0.0), 0.0, 1e-12);
}

TEST(Math, Sin) { EXPECT_NEAR(m::sin(0.5), std::sin(0.5), 1e-9); }

TEST(Math, QuatVecRoundtrip) {
	m::Vector4 v;
	v << 1, 2, 3, 4;
	const auto q  = m::vec2quat(v);
	const auto v2 = m::quat2vec(q);
	EXPECT_TRUE(v.isApprox(v2));
}
