// Contingency-scenario branching: the same flight simulated under different
// failure modes (parachute failure -> ballistic, early motor cutoff, CATO).
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
		const std::string path = "/tmp/trochia_test_scenario.eng";
		std::ofstream o(path);
		o << "TestMotor 112 1150 P 1.0 2.0 Test\n"
		  << "0.0 0.0\n0.1 500.0\n1.0 500.0\n2.0 0.0\n";
		return path;
	}

	simulation::Simulation make_sim() {
		simulation::Simulation sim;
		sim.dt = 0.01; sim.wind_speed = 2.0; sim.wind_dir = 90.0;
		sim.launcher = environment::Launcher(5.0, 90.0, 89.0);
		auto &r = sim.rocket;
		r.diameter = 0.112; r.length = 1.15; r.mass = 5.0;
		r.lcg0 = 0.90; r.lcgf = 0.90; r.lcgp = 1.00; r.lcp = 1.10;
		r.I0 = 1.2; r.If = 1.2; r.Cd = 0.44; r.Cna = 7.84;
		r.engine.load_eng(write_eng());
		r.Cmq = -1.0 * r.Cna / 2.0 * std::pow((r.lcp - r.lcg0) / r.length, 2.0);
		r.parachutes.push_back(rocket::Parachute{
			.at_apogee = true, .delay = 0.0, .cd = 1.5, .area = math::pi * 1.0 * 1.0 / 4.0});
		r.time = 0.0;
		r.pos.vec.setZero(); r.vel.vec.setZero(); r.acc.vec.setZero();
		r.omega.setZero(); r.domega.setZero();
		r.quat = sim.launcher.get_angle();
		return sim;
	}

	struct Outcome { double apogee, landing_speed; bool chute_open; };

	Outcome fly(const simulation::Scenario &sc) {
		auto sim = make_sim();
		sim.scenario = sc;
		auto solve = solver::RK4(sim.rocket, rocket::Rocket::dx);
		Outcome o{0.0, 0.0, false};
		for (int i = 0; i < 200000; ++i) {
			simulation::do_step(sim, solve);
			const coordinate::local::NED p = sim.rocket.pos;
			o.apogee = std::max(o.apogee, p.altitude());
			if (p.vec.norm() > sim.launcher.length && p.altitude() <= 0.0) {
				o.landing_speed = sim.rocket.vel.vec.norm();
				o.chute_open = sim.rocket.chute_open();
				break;
			}
		}
		return o;
	}
}

TEST(SimScenario, NominalDeploysChute) {
	const auto nom = fly(simulation::Scenario{});
	EXPECT_TRUE(nom.chute_open) << "nominal flight should deploy the chute";
}

// Recovery failure: the chute is configured but does not deploy -> ballistic,
// so the landing is far faster than the nominal (chute) descent.
TEST(SimScenario, RecoveryFailureIsBallistic) {
	const auto nom = fly(simulation::Scenario{});
	simulation::Scenario f; f.name = "chute-failure"; f.recovery_fail = true;
	const auto fail = fly(f);

	EXPECT_FALSE(fail.chute_open) << "recovery failure must not deploy the chute";
	EXPECT_GT(fail.landing_speed, 3.0 * nom.landing_speed)
		<< "ballistic landing (" << fail.landing_speed
		<< ") should be far faster than chute landing (" << nom.landing_speed << ")";
}

// Early motor cutoff delivers less impulse -> lower apogee.
TEST(SimScenario, MotorCutoffLowersApogee) {
	const auto nom = fly(simulation::Scenario{});
	simulation::Scenario c; c.name = "motor-cutoff"; c.motor_cutoff = 0.5;
	const auto cut = fly(c);

	EXPECT_LT(cut.apogee, nom.apogee) << "cutoff at 0.5 s should lower the apogee";
	EXPECT_TRUE(cut.chute_open) << "an intact airframe still recovers after a cutoff";
}

// CATO: structural failure -> thrust stops and no recovery (ballistic, low).
TEST(SimScenario, CatoIsLowAndBallistic) {
	const auto nom = fly(simulation::Scenario{});
	simulation::Scenario c; c.name = "cato"; c.cato = 0.5;
	const auto cato = fly(c);

	EXPECT_LT(cato.apogee, nom.apogee)      << "CATO at 0.5 s should lower the apogee";
	EXPECT_FALSE(cato.chute_open)           << "CATO disables recovery";
	// ballistic (no chute), though slower than a full-impulse fall since the
	// early cutoff gives a much lower apogee
	EXPECT_GT(cato.landing_speed, 2.0 * nom.landing_speed) << "CATO descent is ballistic";
}

// A CATO after the chute has already deployed must destroy it -> revert to
// ballistic (recovery is only honoured up to the CATO time, not from t=0).
TEST(SimScenario, CatoAfterDeploymentRevertsToBallistic) {
	const auto nom = fly(simulation::Scenario{});       // full chute descent
	simulation::Scenario c; c.name = "late-cato"; c.cato = 12.0;  // after apogee+deploy
	const auto late = fly(c);

	EXPECT_FALSE(late.chute_open) << "CATO must destroy an already-open chute";
	EXPECT_GT(late.landing_speed, 3.0 * nom.landing_speed)
		<< "after the CATO the descent is ballistic again";
}
