#include <gtest/gtest.h>
#include <coordinate.hpp>

using namespace trochia;
using namespace trochia::coordinate;

TEST(local, NED){
	const auto ned = local::NED(1.0, 2.0, 3.0);

	EXPECT_FLOAT_EQ(ned.north(), 1.0);
	EXPECT_FLOAT_EQ(ned.east(), 2.0);
	EXPECT_FLOAT_EQ(ned.down(), 3.0);

	EXPECT_FLOAT_EQ(ned.south(), -1.0);
	EXPECT_FLOAT_EQ(ned.west(), -2.0);
	EXPECT_FLOAT_EQ(ned.up(), -3.0);
}

TEST(local, ENU){
	const auto enu = local::ENU(1.0, 2.0, 3.0);

	EXPECT_FLOAT_EQ(enu.east(), 1.0);
	EXPECT_FLOAT_EQ(enu.north(), 2.0);
	EXPECT_FLOAT_EQ(enu.up(), 3.0);

	EXPECT_FLOAT_EQ(enu.west(), -1.0);
	EXPECT_FLOAT_EQ(enu.south(), -2.0);
	EXPECT_FLOAT_EQ(enu.down(), -3.0);
}

TEST(local, cast){
	const auto ned = local::NED(1.0, 2.0, 3.0);
	const local::ENU enu = ned;

	EXPECT_FLOAT_EQ(enu.north(), 1.0);
	EXPECT_FLOAT_EQ(enu.east(), 2.0);
	EXPECT_FLOAT_EQ(enu.down(), 3.0);
}

TEST(body, to_local){
	const auto body = body::Body(1.0, 2.0, 3.0);
	auto q = math::euler2quat(0.0, 0.0, 0.0);
	q.normalize();

	// 基準の角度では機体座標系のXYZがNEDのXYZと同じ
	auto ned = body.to_local<local::NED>(q);
	EXPECT_FLOAT_EQ(ned.north(), 1.0);
	EXPECT_FLOAT_EQ(ned.east(), 2.0);
	EXPECT_FLOAT_EQ(ned.down(), 3.0);

	// ロール90°
	q = math::euler2quat(math::pi*0.5, 0.0, 0.0);
	q.normalize();

	ned = body.to_local<local::NED>(q);
	EXPECT_FLOAT_EQ(ned.north(), 1.0);
	EXPECT_FLOAT_EQ(ned.down(), 2.0);
	EXPECT_FLOAT_EQ(ned.west(), 3.0);

	// ピッチ90°
	q = math::euler2quat(0.0, math::pi*0.5, 0.0);
	q.normalize();

	ned = body.to_local<local::NED>(q);
	EXPECT_FLOAT_EQ(ned.up(), 1.0);
	EXPECT_FLOAT_EQ(ned.east(), 2.0);
	EXPECT_FLOAT_EQ(ned.north(), 3.0);

	// ヨー90°
	q = math::euler2quat(0.0, 0.0, math::pi*0.5);
	q.normalize();

	ned = body.to_local<local::NED>(q);
	EXPECT_FLOAT_EQ(ned.east(), 1.0);
	EXPECT_FLOAT_EQ(ned.south(), 2.0);
	EXPECT_FLOAT_EQ(ned.down(), 3.0);
}
