// The launch rail's length and azimuth come from the config ([launcher] length
// / azimuth). main.cpp builds the Launcher for each swept elevation via
// Simulation::make_launcher(); these tests pin that mapping, including the
// backward-compatible defaults used when the config omits the fields.
#include <gtest/gtest.h>
#include "simulation.hpp"

using namespace trochia;

namespace {
	// quat<->euler round-trips cleanly away from the 90-deg gimbal singularity.
	constexpr double TOL = 1e-4;
}

// Without launcher.length / launcher.azimuth in the config, keep the historical
// hardcoded behaviour (5 m rail, 150 deg azimuth).
TEST(LauncherConfig, DefaultsMatchLegacyHardcode) {
	simulation::Simulation sim;
	auto l = sim.make_launcher(70.0);
	EXPECT_DOUBLE_EQ(l.length, 5.0);
	EXPECT_NEAR(l.azimuth(),   150.0, TOL);
	EXPECT_NEAR(l.elevation(),  70.0, TOL);
}

// Configured rail length and azimuth flow through to the constructed Launcher,
// with the swept elevation applied per run.
TEST(LauncherConfig, HonorsConfiguredLengthAndAzimuth) {
	simulation::Simulation sim;
	sim.launcher_length  = 12.0;   // Astra: 12 m rail
	sim.launcher_azimuth = 133.0;  // Astra: heading 133 deg
	auto l = sim.make_launcher(84.0);
	EXPECT_DOUBLE_EQ(l.length, 12.0);
	EXPECT_NEAR(l.azimuth(),   133.0, TOL);
	EXPECT_NEAR(l.elevation(),  84.0, TOL);
}
