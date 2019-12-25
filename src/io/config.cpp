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

auto load(const std::string &fname, simulation::Simulation &sim) -> Conditions {
	using namespace toml;

	cond_elevation	launcher_elevation;
	cond_wspeed		wind_speed;
	cond_wdir		wind_dir;

	const auto config = parse(fname);

	// simulation common
	{
		const auto &cfg_sim = find(config, "simulation");
		sim.dt = cfg_sim.at("dt").as_floating();

		const auto output = find(cfg_sim, "output");
		sim.output_dt = output.at("dt").as_floating();

		sim.output_dir_fmt = output.at("dir").as_string();
	}

	// launcher info
	{
		const auto &cfg_launcher = find(config, "launcher");

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
	}

	return {launcher_elevation, wind_speed, wind_dir};
}

}
