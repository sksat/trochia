#include <gtest/gtest.h>
#include <environment.hpp>

using namespace trochia;
using namespace trochia::environment;

TEST(wind, speed){
	EXPECT_EXIT(wind::speed(0.0, 0.0, 0.0, 0.0), ::testing::KilledBySignal(SIGABRT), "");
}

TEST(wind, wind){
	auto w = wind::wind(1.0, 0.0);
	EXPECT_FLOAT_EQ(w.north(), 1.0);

	w = wind::wind(1.0, math::pi*0.5);
	EXPECT_FLOAT_EQ(w.east(), 1.0);
}
