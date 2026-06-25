// Exercises simulation::do_step so the sanitizers cover the flight dynamics.
//
// Regression test for issue #37: do_step held the airspeed as an Eigen
// expression template (`auto va_body = ned2body(quat) * va_ned`) that referenced
// the temporary matrix returned by ned2body(); reading it afterwards is a
// stack-use-after-scope (caught here under -fsanitize=address). The trajectory
// must also stay finite.
#include <gtest/gtest.h>
#include <cmath>
#include <fstream>
#include <string>
#include "simulation.hpp"
#include "solver.hpp"

using namespace trochia;

namespace {
	std::string write_eng() {
		const std::string path = "/tmp/trochia_test_sim.eng";
		std::ofstream o(path);
		o << "TestMotor 112 1150 P 1.0 2.0 Test\n"
		  << "0.0 0.0\n"
		  << "0.1 500.0\n"
		  << "1.0 500.0\n"
		  << "2.0 0.0\n";
		return path;
	}

	simulation::Simulation make_sim() {
		simulation::Simulation sim;
		sim.dt        = 0.01;
		sim.wind_speed = 3.0;
		sim.wind_dir   = -90.0;
		sim.launcher   = environment::Launcher(5.0, 90.0, 85.0);

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

TEST(SimDynamics, DoStepStaysFinite) {
	auto sim = make_sim();
	auto solve = solver::RK4(sim.rocket, rocket::Rocket::dx);

	// a non-trivial upward velocity so the airspeed vector is exercised
	sim.rocket.vel.vec = math::Vector3(0.0, 0.0, -30.0); // NED: down is +, so up

	const double start_alt = sim.rocket.pos.altitude();

	for (int i = 0; i < 50; ++i) {
		trochia::simulation::do_step(sim, solve);
		ASSERT_TRUE(std::isfinite(sim.rocket.pos.altitude())) << "altitude NaN at step " << i;
		ASSERT_TRUE(std::isfinite(sim.rocket.vel.vec.norm())) << "velocity NaN at step " << i;

		// The launch is wholly in the East-Up plane: azimuth 90 deg, wind along
		// the E axis (wind_dir -90), initial velocity straight up. Nothing forces
		// out-of-plane (north) motion, so north must stay zero. Issue #37's
		// uninitialised state and tumble leaked numerical noise into a growing
		// out-of-plane drift -- a finiteness check alone would not have caught it.
		EXPECT_NEAR(sim.rocket.pos.north(), 0.0, 1e-9)
			<< "out-of-plane (north) drift at step " << i;
	}

	// 500 N thrust on a 5 kg rocket plus the initial 30 m/s climb: altitude must grow.
	EXPECT_GT(sim.rocket.pos.altitude(), start_alt) << "rocket did not ascend under thrust";
}
