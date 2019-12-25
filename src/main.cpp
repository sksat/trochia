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
#include <vector>
#include <string>
#include <string_view>
#include <filesystem>
#include <toml.hpp>
#include "version.hpp"
#include "simulation.hpp"
#include "io/config.hpp"

namespace fs = std::filesystem;

auto shrink_str(std::string_view sv) -> std::string_view;
auto make_output_dir(const fs::path &dir) -> bool;

auto main(int argc, char **argv) -> int {
	trochia::simulation::Simulation sim_base;

	trochia::version::version();

	std::cerr << "loading config file ...";
	const auto [elevation, wind_speed, wind_dir]
		= trochia::io::config::load("config.toml", sim_base);
	std::cerr << std::endl;

	std::cerr << "start simulation" << std::endl;

	std::cout << "sim number: "
		<< elevation.size()*wind_speed.size()*wind_dir.size() << std::endl;

	for(const auto &e : elevation){
		const auto launcher = trochia::environment::Launcher(5.0, 90.0, e);
		for(const auto &ws : wind_speed){
			for(const auto &wd  : wind_dir){
				const auto e_str = shrink_str(std::to_string(e));
				const auto ws_str = shrink_str(std::to_string(ws));
				const auto wd_str = shrink_str(std::to_string(wd));

				const auto output_dir = fs::path(sim_base.output_dir_fmt)
					/ e_str / ws_str / wd_str;

				std::cout << "output dir: " << output_dir << std::endl;

				make_output_dir(output_dir);

				auto sim = sim_base;
				sim.output_dir	= output_dir;
				sim.launcher	= launcher;
				sim.wind_speed	= ws;
				sim.wind_dir	= wd;

				trochia::simulation::exec(sim);
			}
		}
	}

	return 0;
}

auto shrink_str(std::string_view sv) -> std::string_view {
	const auto &b = sv.back();
	if(b == '0' || b == '.'){
		sv.remove_suffix(1);

		if(b != '.')
			sv = shrink_str(sv);
	}
	return sv;
}

auto make_output_dir(const fs::path &dir) -> bool {
	if(fs::exists(dir)){
		if(fs::is_directory(dir))
			return true;

		std::cerr << "error: " << dir << " is not directory." << std::endl;
		return false;
	}

	if(!fs::exists(dir.parent_path())){
		const auto p = dir.parent_path();
		if(!p.empty()){
			std::cerr << "parent directory " << p
				<< " does not exist." << std::endl;
			if(!make_output_dir(p))
				return false;
		}
	}

	std::cerr << "creating directory " << dir << "...";
	if(!fs::create_directory(dir)){
		std::cerr << "[failed]" << std::endl;
		return false;
	}
	std::cerr << "[ok]" << std::endl;

	return true;
}
