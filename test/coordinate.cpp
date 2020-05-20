#include <gtest/gtest.h>
#include <coordinate.hpp>

using namespace trochia::coordinate;

TEST(local, NED){
	const auto ned = local::NED(1.0, 2.0, 3.0);

	EXPECT_EQ(ned.north(), 1.0);
	EXPECT_EQ(ned.east(), 2.0);
	EXPECT_EQ(ned.down(), 3.0);

	EXPECT_EQ(ned.south(), -1.0);
	EXPECT_EQ(ned.west(), -2.0);
	EXPECT_EQ(ned.up(), -3.0);
}

TEST(local, ENU){
	const auto enu = local::ENU(1.0, 2.0, 3.0);

	EXPECT_EQ(enu.east(), 1.0);
	EXPECT_EQ(enu.north(), 2.0);
	EXPECT_EQ(enu.up(), 3.0);

	EXPECT_EQ(enu.west(), -1.0);
	EXPECT_EQ(enu.south(), -2.0);
	EXPECT_EQ(enu.down(), -3.0);
}

TEST(local, cast){
	const auto ned = local::NED(1.0, 2.0, 3.0);
	const local::ENU enu = ned;

	EXPECT_EQ(enu.north(), 1.0);
	EXPECT_EQ(enu.east(), 2.0);
	EXPECT_EQ(enu.down(), 3.0);
}
