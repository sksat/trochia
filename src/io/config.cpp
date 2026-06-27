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


#include <iostream>
#define TOML11_COLORIZE_ERROR_MESSAGE
#include <toml.hpp>
#include "../simulation.hpp"
#include "config.hpp"

namespace trochia::io::config {

// read a TOML number as double whether it was written as 1 or 1.0 (toml11 is
// strict: as_floating() throws on an integer literal).
static auto as_number(const toml::value &v) -> double {
	return v.is_integer() ? static_cast<double>(v.as_integer()) : v.as_floating();
}

auto load(const std::string &fname, simulation::Simulation &sim) -> Conditions {
	using namespace toml;

	cond_elevation	launcher_elevation;
	cond_wspeed		wind_speed;
	cond_wdir		wind_dir;
	cond_scenario	scenarios;

	const auto config = parse(fname);

	// simulation common
	{
		const auto &cfg_sim = find(config, "simulation");
		sim.dt = cfg_sim.at("dt").as_floating();

		// optional: simulation time limit [s] (default 60). A parachute descent
		// from km altitude takes minutes, so it usually needs raising.
		if(cfg_sim.contains("timeout"))
			sim.timeout = as_number(cfg_sim.at("timeout"));

		const auto output = find(cfg_sim, "output");
		sim.output_dt = output.at("dt").as_floating();

		sim.output_dir_fmt = output.at("dir").as_string();
	}

	// launcher info
	{
		const auto &cfg_launcher = find(config, "launcher");

		// optional rail length [m] and azimuth [deg]. Omitted -> historical
		// hardcoded defaults (5 m, 150 deg); see Simulation::make_launcher.
		if(cfg_launcher.contains("length"))
			sim.launcher_length = as_number(cfg_launcher.at("length"));
		if(cfg_launcher.contains("azimuth"))
			sim.launcher_azimuth = as_number(cfg_launcher.at("azimuth"));

		// get elevation
		const auto elevation = find(cfg_launcher, "elevation");
		if(elevation.is_integer())
			launcher_elevation.push_back(elevation.as_integer());
		else if(elevation.is_floating())
			launcher_elevation.push_back(elevation.as_floating());
		else if(elevation.is_table()){
			const auto start = find(elevation, "start");
			const auto end   = find(elevation, "end");
			if(start.is_integer() && end.is_integer()){
				for(int a=start.as_integer();a<=end.as_integer();a++)
					launcher_elevation.push_back(a);
			}else{
				std::cerr << "error: elevation.start or elevation.end are not integer" << std::endl;
			}
		}else if(elevation.is_array()){
			launcher_elevation = get<std::vector<double>>(elevation);
		}
	}

	// wind info
	{
		const auto &cfg_wind = find(config, "wind");

		// get model
		const auto model = cfg_wind.at("model").as_string();
		if(model == "power"){
			// get ground wind
			const auto ground = find(cfg_wind, "ground");

			const auto ws	= find(ground, "speed");
			const auto wd	= find(ground, "dir");

			if(ws.is_floating())
				wind_speed.push_back(ws.as_floating());
			else if(ws.is_array())
				wind_speed = get<std::vector<double>>(ws);

			if(wd.is_floating())
				wind_dir.push_back(wd.as_floating());
			else if(wd.is_array())
				wind_dir = get<std::vector<double>>(wd);
		}else{
			std::cerr << "error: wind model \"" << model << "\" is not implemented." << std::endl;
		}
	}

	// rocket info
	{
		const auto &cfg_rkt = find(config, "rocket");
		const auto stage = find<std::vector<toml::table>>(cfg_rkt, "stage");
		if(stage.size() != 1){
			std::cerr
				<< "multistage rocket is not implemented now. sorry."
				<< std::endl;
		}

		const auto engine = stage[0].at("engine");
		sim.rocket.engine.load_eng(engine.as_string());

		sim.rocket.diameter = stage[0].at("diameter").as_floating();
		sim.rocket.length	= stage[0].at("length").as_floating();

		sim.rocket.mass		= stage[0].at("mass").as_floating();

		sim.rocket.lcg0 = stage[0].at("lcg0").as_floating();
		sim.rocket.lcgf = stage[0].at("lcgf").as_floating();
		sim.rocket.lcgp = stage[0].at("lcgp").as_floating();

		sim.rocket.lcp	= stage[0].at("lcp").as_floating();

		sim.rocket.I0	= stage[0].at("I0").as_floating();
		sim.rocket.If	= stage[0].at("If").as_floating();

		sim.rocket.Cd = stage[0].at("Cd").as_floating();
		sim.rocket.Cna= stage[0].at("Cna").as_floating();

		// recovery: one or more parachute stages. `parachute` may be a single
		// inline table or an array of tables (drogue, main, ...). A stage with an
		// `altitude` deploys at that AGL on the way down; otherwise it deploys at
		// apogee (+ optional `delay`). cd + diameter set its drag.
		sim.rocket.parachutes.clear();	// overwrite, so load() is idempotent
		if(stage[0].contains("parachute")){
			const auto parse_chute = [](const toml::value &t) -> rocket::Parachute {
				rocket::Parachute p;
				if(t.contains("altitude"))
					p.altitude = as_number(t.at("altitude"));
				else {
					p.at_apogee = true;
					if(t.contains("delay"))
						p.delay = as_number(t.at("delay"));
				}
				if(t.contains("cd"))
					p.cd = as_number(t.at("cd"));
				if(t.contains("diameter")){
					const auto d = as_number(t.at("diameter"));
					p.area = math::pi * d * d / 4.0;
				}
				return p;
			};
			// only keep stages that actually have drag (cd>0 and a diameter);
			// an incomplete stage means no recovery (ballistic), as before.
			const auto add_chute = [&](const toml::value &t){
				const auto p = parse_chute(t);
				if(p.cd > 0.0 && p.area > 0.0)
					sim.rocket.parachutes.push_back(p);
			};
			const auto &pc = stage[0].at("parachute");
			if(pc.is_array())
				for(const auto &t : pc.as_array())
					add_chute(t);
			else
				add_chute(pc);
		}
	}

	// contingency scenarios (optional). Each [[scenario]] is a failure branch;
	// without any, a single nominal flight is run.
	if(config.contains("scenario")){
		const auto arr = find<std::vector<toml::table>>(config, "scenario");
		for(const auto &s : arr){
			simulation::Scenario sc;
			if(s.count("name"))
				sc.name = s.at("name").as_string();
			if(s.count("recovery"))
				sc.recovery_fail = (s.at("recovery").as_string() == "none");
			if(s.count("motor_cutoff"))
				sc.motor_cutoff = as_number(s.at("motor_cutoff"));
			if(s.count("cato"))
				sc.cato = as_number(s.at("cato"));
			scenarios.push_back(sc);
		}
	}
	if(scenarios.empty())
		scenarios.push_back(simulation::Scenario{});	// nominal

	return {launcher_elevation, wind_speed, wind_dir, scenarios};
}

}
