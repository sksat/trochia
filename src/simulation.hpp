/* ----------------------------------------------------------------------- *
 *
 *    Copyright (C) 2019 sksat <sksat@sksat.net>
 *
 *    This file is part of Trochia.
 *
 *    Trochia is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Trochia is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You Should have received a copy of the GNU General Public License
 *    along with Trochia.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ----------------------------------------------------------------------- */

#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

#include <utility>
#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include <fstream>
#include "math.hpp"
#include "environment/launcher.hpp"
#include "rocket.hpp"
#include "solver.hpp"

namespace trochia::simulation {
	using math::Float;

	// A contingency branch to simulate (failure-mode scenarios). The nominal
	// flight is the default; the fields override it to model a failure.
	struct Scenario {
		std::string name = "nominal";
		bool recovery_fail = false;        // parachute does not deploy -> ballistic
		std::optional<Float> motor_cutoff; // thrust = 0 after t [s] (airframe intact)
		std::optional<Float> cato;         // structural failure at t [s]: thrust=0 + no recovery
	};

	class Simulation {
	public:
		// simulation config
		Float timeout = 60.0;
		Float dt, output_dt;

		std::string output_dir_fmt;
		std::filesystem::path output_dir;

		// input value
		environment::Launcher launcher;
		// launch rail, from the config [launcher] table. Defaults preserve the
		// historical hardcoded rail (5 m, azimuth 150 deg) when the config omits
		// them; elevation is swept per run and supplied to make_launcher().
		Float launcher_length  = 5.0;
		Float launcher_azimuth = 150.0;

		Float wind_speed, wind_dir;

		Scenario scenario;	// active contingency branch (default: nominal)

		rocket::Rocket rocket;

		// output value
		std::pair<Float,Float> launch_clear;
		Float geo_height;
		Float temperature;
		Float rho;

		struct GHP {
			Float wspeed, wdir;
			Float e, n, u;
			Float max_altitude;
		};
		GHP ghp_local;

		// Build the launch rail for a given elevation [deg] from the configured
		// length and azimuth. main.cpp calls this once per swept elevation.
		auto make_launcher(const Float &elevation_deg) const -> environment::Launcher {
			return environment::Launcher(launcher_length, launcher_azimuth, elevation_deg);
		}
	};

	auto exec(Simulation &sim) -> void;
	auto do_step(Simulation &sim, solver::solver<rocket::Rocket> &solve) -> void;
	auto save_data(const math::Float &time, const Simulation &sim, std::vector<std::ofstream> &output) -> void;
}

#endif
