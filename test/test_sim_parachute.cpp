// Parachute / recovery model (issue #6).
//
// Once the chute deploys (at apogee + delay), the rocket hangs under the canopy:
// it descends at the chute's terminal velocity and drifts with the wind. That
// descent -- not a ballistic fall -- is what makes a recovered rocket's landing
// footprint realistic. These tests check the terminal velocity, the wind drift,
// and that deployment actually slows the descent versus ballistic.
#include <gtest/gtest.h>
#include <cmath>
#include <fstream>
#include <string>
#include "simulation.hpp"
#include "solver.hpp"
#include "coordinate.hpp"
// NOTE: we deliberately don't include the environment headers here -- several
// define non-inline functions and clash with simulation.cpp at link time -- so
// gravity and the wind power law are recomputed locally in the tests below.

using namespace trochia;

namespace {
	std::string write_eng() {
		const std::string path = "/tmp/trochia_test_chute.eng";
		std::ofstream o(path);
		o << "TestMotor 112 1150 P 1.0 2.0 Test\n"
		  << "0.0 0.0\n0.1 500.0\n1.0 500.0\n2.0 0.0\n";
		return path;
	}

	// rocket with a parachute (cd 1.5, 1.0 m diameter)
	simulation::Simulation make_sim() {
		simulation::Simulation sim;
		sim.dt = 0.01;
		sim.wind_speed = 0.0;
		sim.wind_dir = 0.0;
		sim.launcher = environment::Launcher(5.0, 90.0, 89.0);

		auto &r = sim.rocket;
		r.diameter = 0.112; r.length = 1.15; r.mass = 5.0;
		r.lcg0 = 0.90; r.lcgf = 0.90; r.lcgp = 1.00; r.lcp = 1.10;
		r.I0 = 1.2; r.If = 1.2; r.Cd = 0.44; r.Cna = 7.84;
		r.engine.load_eng(write_eng());
		r.Cmq = -1.0 * r.Cna / 2.0 * std::pow((r.lcp - r.lcg0) / r.length, 2.0);

		// single chute at apogee, 1.0 m diameter
		r.parachutes.push_back(rocket::Parachute{
			.at_apogee = true, .delay = 0.0, .cd = 1.5, .area = math::pi * 1.0 * 1.0 / 4.0});

		r.time = 0.0;
		r.pos.vec.setZero();
		r.vel.vec.setZero();
		r.acc.vec.setZero();
		r.omega.setZero();
		r.domega.setZero();
		r.quat = sim.launcher.get_angle();
		return sim;
	}

	// place the rocket high up, descending under an already-open chute
	void start_under_chute(simulation::Simulation &sim, double altitude) {
		auto &r = sim.rocket;
		r.pos.vec.setZero();
		r.pos.up(altitude);
		r.vel.vec.setZero();
		r.parachutes[0].open = true;
		r.apogee_time = 0.0;   // already past apogee
	}
}

// Vertical descent settles at the analytic terminal velocity
// v_t = sqrt(2 W g / (rho Cd A)).
TEST(SimParachute, ReachesTerminalVelocity) {
	auto sim = make_sim();
	start_under_chute(sim, 1000.0);
	auto solve = solver::RK4(sim.rocket, rocket::Rocket::dx);

	for (int i = 0; i < 1500; ++i)   // ~15 s: long enough to settle
		simulation::do_step(sim, solve);

	const double W  = sim.rocket.weight();
	const double g  = 9.80665;   // gravity at ~km altitude differs <0.1% from g0
	const double vt = std::sqrt(2.0 * W * g / (sim.rho * sim.rocket.chute_cda()));

	const coordinate::local::NED v = sim.rocket.vel;
	EXPECT_GT(v.vec.z(), 0.0) << "should be descending (NED down +)";
	EXPECT_NEAR(v.vec.z(), vt, 0.05 * vt) << "descent rate is not the chute terminal velocity";
}

// Under the chute the horizontal velocity is driven toward the wind (drift).
TEST(SimParachute, DriftsWithWind) {
	auto sim = make_sim();
	sim.wind_speed = 5.0;
	sim.wind_dir   = 90.0;   // NED: dir 90 deg -> +East
	start_under_chute(sim, 1000.0);
	auto solve = solver::RK4(sim.rocket, rocket::Rocket::dx);

	for (int i = 0; i < 2000; ++i)
		simulation::do_step(sim, solve);

	// wind power law (matches environment::wind::speed(n=6, z_r=2, ...))
	const double alt  = sim.rocket.pos.altitude();
	const double wind = sim.wind_speed * std::pow(alt / 2.0, 1.0 / 6.0);
	const coordinate::local::NED v = sim.rocket.vel;
	EXPECT_GT(v.vec.y(), 0.0) << "should drift east (toward the wind)";
	EXPECT_LT(std::fabs(v.vec.x()), 0.1 * wind) << "no north component expected";
	EXPECT_GT(v.vec.y(), 0.7 * wind) << "horizontal velocity should approach the wind speed";
}

// Dual deployment: a small drogue at apogee (fast descent) then a large main at
// a set altitude (slow descent) -- the descent rate drops once the main opens.
TEST(SimParachute, DualDeployFastUnderDrogueThenSlowUnderMain) {
	auto sim = make_sim();
	auto &r = sim.rocket;
	r.parachutes.clear();
	r.parachutes.push_back(rocket::Parachute{           // drogue at apogee (small)
		.at_apogee = true, .delay = 0.0, .cd = 1.2, .area = math::pi * 0.3 * 0.3 / 4.0});
	r.parachutes.push_back(rocket::Parachute{           // main at 300 m AGL (large)
		.altitude = 300.0, .cd = 1.5, .area = math::pi * 2.0 * 2.0 / 4.0});
	r.pos.vec.setZero(); r.pos.up(900.0);
	r.vel.vec.setZero();
	r.apogee_time = 0.0;
	r.parachutes[0].open = true;                        // drogue already out

	auto solve = solver::RK4(sim.rocket, rocket::Rocket::dx);
	double v_drogue = 0.0, v_main = 0.0;
	bool main_seen = false;
	for (int i = 0; i < 200000; ++i) {
		simulation::do_step(sim, solve);
		const coordinate::local::NED p = sim.rocket.pos;
		const double alt = p.altitude(), vz = sim.rocket.vel.vec.z();
		if (alt > 500.0 && alt < 700.0) v_drogue = vz;  // under drogue only
		if (sim.rocket.parachutes[1].open) {
			main_seen = true;
			if (alt > 100.0 && alt < 250.0) v_main = vz; // under drogue + main
		}
		if (p.vec.norm() > sim.launcher.length && alt <= 0.0) break;
	}
	EXPECT_TRUE(main_seen) << "the main should deploy below its altitude";
	EXPECT_GT(v_drogue, 0.0);
	EXPECT_GT(v_main, 0.0);
	EXPECT_GT(v_drogue, 1.5 * v_main)
		<< "drogue descent (" << v_drogue << ") should be faster than under the main ("
		<< v_main << ")";
}

// Deploying the chute makes the rocket land far slower than a ballistic fall.
TEST(SimParachute, DeploymentSlowsDescentVsBallistic) {
	auto landing_speed = [](bool with_chute) {
		auto sim = make_sim();
		if (!with_chute) sim.rocket.parachutes.clear();   // ballistic
		auto solve = solver::RK4(sim.rocket, rocket::Rocket::dx);
		double speed = 0.0;
		for (int i = 0; i < 100000; ++i) {
			simulation::do_step(sim, solve);
			const coordinate::local::NED p = sim.rocket.pos;
			if (p.vec.norm() > sim.launcher.length && p.altitude() <= 0.0) {
				speed = sim.rocket.vel.vec.norm();
				break;
			}
		}
		return std::make_pair(speed, sim.rocket.chute_open());
	};

	const auto [v_chute, opened] = landing_speed(true);
	const auto [v_ball, _]       = landing_speed(false);

	EXPECT_TRUE(opened) << "the chute should have deployed during the flight";
	EXPECT_GT(v_chute, 0.0);
	EXPECT_GT(v_ball, 0.0);
	EXPECT_LT(v_chute, 0.5 * v_ball)
		<< "chute landing speed " << v_chute << " should be far below ballistic " << v_ball;
}
