// Engine thrust-curve lookup must be correct and call-order independent.
//
// Regression test for issue #37: thrust() advanced a member iterator (stateful),
// so the result depended on call order, load_eng left the iterator mid-curve,
// and after a Simulation copy the iterator pointed into another container
// (UB, heap-address dependent -> run-to-run nondeterminism).
#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "engine.hpp"

using trochia::Engine;

namespace {
	// header: name diameter length delays prop_weight total_weight manufacturer
	std::string write_eng() {
		const std::string path = "/tmp/trochia_test_motor.eng";
		std::ofstream o(path);
		o << "TestMotor 24 70 P 0.1 0.2 Test\n"
		  << "0.0 0.0\n"
		  << "1.0 10.0\n"
		  << "2.0 10.0\n"
		  << "3.0 0.0\n";
		return path;
	}
}

class EngineTest : public ::testing::Test {
protected:
	Engine eng;
	void SetUp() override { eng.load_eng(write_eng()); }
};

TEST_F(EngineTest, Interpolates) {
	EXPECT_NEAR(eng.thrust(0.5), 5.0, 1e-9);   // between (0,0)-(1,10)
	EXPECT_NEAR(eng.thrust(1.5), 10.0, 1e-9);  // flat top
	EXPECT_NEAR(eng.thrust(2.5), 5.0, 1e-9);   // between (2,10)-(3,0)
}

TEST_F(EngineTest, ZeroAfterBurnout) {
	EXPECT_NEAR(eng.thrust(3.0), 0.0, 1e-9);
	EXPECT_NEAR(eng.thrust(10.0), 0.0, 1e-9);
}

// The crucial property: querying out of order yields identical results.
TEST_F(EngineTest, CallOrderIndependent) {
	const double a = eng.thrust(0.5);
	eng.thrust(2.9);   // jump near the end
	eng.thrust(0.1);   // jump back
	const double b = eng.thrust(0.5);
	EXPECT_DOUBLE_EQ(a, b);
	EXPECT_NEAR(b, 5.0, 1e-9);
}

// A copied Engine must behave identically (Simulation copies it per case).
TEST_F(EngineTest, CopyIsConsistent) {
	const Engine copy = eng;
	for (double t = 0.0; t <= 3.0; t += 0.13)
		EXPECT_DOUBLE_EQ(copy.thrust(t), eng.thrust(t)) << "t=" << t;
}
