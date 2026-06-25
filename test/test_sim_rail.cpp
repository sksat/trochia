// Launch-rail constraint test (issue #32: "ランチャをランチャにする").
//
// While the rocket is still on the launch rail it is mechanically constrained
// to the rail axis: the rail reacts every force perpendicular to itself (the
// normal reaction / 垂直抗力), so the rocket may only translate along the rail.
//
// Before the fix only the rotation was frozen on the rail; the translational
// equations still applied the full 3-DOF force. The perpendicular component of
// gravity (a tilted rail) and the spurious low-speed angle of attack therefore
// pushed the rocket sideways off the rail before it cleared:
//     elevation 80 deg : ~0.15 m off-axis at launch clear
//     elevation 85 deg : ~0.07 m
// With the constraint the off-axis displacement is machine-zero (~1e-15 m) while
// the along-rail launch-clear velocity is unchanged. This catches a regression
// (red ~0.15 m -> green ~0) and is independent of the moment-sign physics.
#include <gtest/gtest.h>
#include <cmath>
#include <fstream>
#include <string>
#include "simulation.hpp"
#include "solver.hpp"
#include "coordinate.hpp"

using namespace trochia;

namespace {
	std::string write_eng() {
		const std::string path = "/tmp/trochia_test_rail.eng";
		std::ofstream o(path);
		o << "TestMotor 112 1150 P 1.0 2.0 Test\n"
		  << "0.0 0.0\n"
		  << "0.1 500.0\n"
		  << "1.0 500.0\n"
		  << "2.0 0.0\n";
		return path;
	}

	simulation::Simulation make_sim(double elevation, double wind) {
		simulation::Simulation sim;
		sim.dt         = 0.01;
		sim.wind_speed = wind;
		sim.wind_dir   = -90.0;
		sim.launcher   = environment::Launcher(5.0, 90.0, elevation);

		auto &r = sim.rocket;
		r.diameter = 0.112; r.length = 1.15; r.mass = 5.0;
		r.lcg0 = 0.90; r.lcgf = 0.90; r.lcgp = 1.00; r.lcp = 1.10;
		r.I0 = 1.2; r.If = 1.2; r.Cd = 0.44; r.Cna = 7.84;
		r.engine.load_eng(write_eng());
		r.Cmq = -1.0 * r.Cna / 2.0 * std::pow((r.lcp - r.lcg0) / r.length, 2.0);

		r.time = 0.0;
		r.pos.vec.setZero();
		r.vel.vec.setZero();
		r.acc.vec.setZero();
		r.omega.setZero();
		r.domega.setZero();
		r.quat = sim.launcher.get_angle();
		return sim;
	}
}

// The rocket must ride the rail exactly until it clears it.
class SimRail : public ::testing::TestWithParam<std::pair<double, double>> {};

TEST_P(SimRail, StaysOnRailUntilLaunchClear) {
	const auto [elevation, wind] = GetParam();
	auto sim = make_sim(elevation, wind);
	auto solve = solver::RK4(sim.rocket, rocket::Rocket::dx);

	// rail axis (body +x at the launcher attitude) expressed in NED
	const math::Vector3 rail =
		coordinate::dcm::body2ned(sim.launcher.get_angle()) * math::Vector3(1.0, 0.0, 0.0);

	double max_perp = 0.0;
	bool cleared = false;
	double clear_speed = 0.0;
	for (int i = 0; i < 1000; ++i) {
		simulation::do_step(sim, solve);
		const auto &r = sim.rocket;
		if (r.pos.vec.norm() > sim.launcher.length) {
			cleared = true;
			clear_speed = r.vel.vec.norm();
			break;
		}
		// distance from the rail line = component of position perpendicular to rail
		const math::Vector3 p = r.pos.vec;
		const math::Vector3 perp = p - rail * p.dot(rail);
		max_perp = std::max(max_perp, perp.norm());
	}

	ASSERT_TRUE(cleared) << "rocket never left the rail (elev=" << elevation
	                     << ", wind=" << wind << ")";          // guards against a trivial pass
	EXPECT_LT(max_perp, 1e-9)
		<< "rocket drifted " << max_perp << " m off the rail axis while still on the rail"
		<< " (elev=" << elevation << ", wind=" << wind << ")";
	EXPECT_GT(clear_speed, 1.0) << "implausibly small launch-clear speed";
}

INSTANTIATE_TEST_SUITE_P(
	Configs, SimRail,
	::testing::Values(
		std::make_pair(85.0, 3.0),
		std::make_pair(80.0, 5.0)));
