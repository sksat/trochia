// Stability regression test for the aerodynamic pitching-moment sign (issue #37).
//
// A statically stable rocket (centre of pressure aft of centre of gravity,
// lcp > lcg) must produce a *restoring* aerodynamic moment: an angle of attack
// induced by the wind has to be driven back towards zero (weathercocking), so
// during powered ascent the angle of attack stays small and bounded.
//
// The moment about the body Y axis was written with the wrong sign
//     ma_y =  lcp_lcg * N          // de-stabilising
// instead of
//     ma_y = -lcp_lcg * N          // restoring
// which turns the moment anti-restoring: any disturbance is amplified and the
// rocket tumbles. The fix is verified empirically here (issue author's chosen
// approach) rather than by re-deriving the sign convention by hand.
//
// Measured max |AoA| during ascent (to apogee), per (elevation, wind):
//     wrong sign : ~179-180 deg  (full tumble; apogee collapses to ~130-170 m)
//     fixed sign :    4-16 deg   (weathercocks; apogee ~390-440 m)
// The 45 deg threshold sits with >3x margin below the stable cases and far
// below the ~180 deg tumble, so this fails (red) on the wrong sign and passes
// (green) on the fix.
//
// NOTE: only the ascent to apogee is examined. On the ballistic descent the
// body still points up while the velocity points down, so AoA legitimately
// approaches 180 deg even for a perfectly stable rocket -- that is not a tumble.
#include <gtest/gtest.h>
#include <cmath>
#include <fstream>
#include <string>
#include "simulation.hpp"
#include "solver.hpp"

using namespace trochia;

namespace {
	std::string write_eng() {
		const std::string path = "/tmp/trochia_test_stability.eng";
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
		sim.wind_dir   = -90.0;                 // wind along the E axis (in-plane)
		sim.launcher   = environment::Launcher(5.0, 90.0, elevation); // azimuth East

		auto &r = sim.rocket;
		r.diameter = 0.112; r.length = 1.15; r.mass = 5.0;
		r.lcg0 = 0.90; r.lcgf = 0.90; r.lcgp = 1.00; r.lcp = 1.10; // CP aft of CG
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

	struct AscentResult {
		double max_aoa_deg = 0.0;   // max |AoA| during ascent after launch clear
		double max_north   = 0.0;   // max |north| (should stay ~0 in-plane)
		double apogee      = 0.0;
		bool   finite      = true;
	};

	// Integrate from the rail until apogee, tracking the stability metrics.
	AscentResult fly_to_apogee(simulation::Simulation &sim) {
		auto solve = solver::RK4(sim.rocket, rocket::Rocket::dx);
		AscentResult res;
		bool cleared = false;
		double prev_alt = -1e9;
		for (int i = 0; i < 5000; ++i) {       // hard cap as a backstop
			simulation::do_step(sim, solve);
			const auto &r = sim.rocket;
			const coordinate::local::NED p = r.pos;
			if (!std::isfinite(p.altitude()) || !std::isfinite(r.vel.vec.norm())) {
				res.finite = false;
				break;
			}
			const double aoa = std::fabs(math::rad2deg(r.angle_attack));
			res.max_north = std::max(res.max_north, std::fabs(p.north()));
			if (p.vec.norm() > sim.launcher.length) cleared = true;
			const bool ascending = p.altitude() > prev_alt;
			if (cleared && ascending) {
				res.max_aoa_deg = std::max(res.max_aoa_deg, aoa);
				res.apogee = std::max(res.apogee, p.altitude());
			}
			if (cleared && !ascending) break;  // apogee reached
			prev_alt = p.altitude();
		}
		return res;
	}
}

// During powered ascent a statically stable rocket weathercocks into the wind
// and the angle of attack stays small. The wrong moment sign tumbles it to
// ~180 deg, which this catches.
class SimStability : public ::testing::TestWithParam<std::pair<double, double>> {};

TEST_P(SimStability, AngleOfAttackStaysBoundedDuringAscent) {
	const auto [elevation, wind] = GetParam();
	auto sim = make_sim(elevation, wind);
	const auto res = fly_to_apogee(sim);

	ASSERT_TRUE(res.finite) << "trajectory went non-finite (elev=" << elevation
	                        << ", wind=" << wind << ")";
	// restoring moment -> bounded AoA; anti-restoring (wrong sign) -> ~180 deg
	EXPECT_LT(res.max_aoa_deg, 45.0)
		<< "rocket appears to tumble during ascent (max AoA " << res.max_aoa_deg
		<< " deg) at elev=" << elevation << ", wind=" << wind
		<< " -- aerodynamic pitching moment is not restoring";
	// in-plane launch (azimuth East, wind along E): no out-of-plane forcing
	EXPECT_LT(res.max_north, 1e-6)
		<< "out-of-plane (north) motion " << res.max_north
		<< " m appeared in a symmetric in-plane case";
}

INSTANTIATE_TEST_SUITE_P(
	Configs, SimStability,
	::testing::Values(
		std::make_pair(89.0, 3.0),
		std::make_pair(85.0, 3.0),
		std::make_pair(80.0, 5.0)));
