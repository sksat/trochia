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
#include <filesystem>
#include <toml.hpp>
#include "version.hpp"
#include "simulation.hpp"
#include "io/config.hpp"

namespace fs = std::filesystem;

auto main(int argc, char **argv) -> int {
	trochia::simulation::Simulation sim;

	trochia::version::version();

	std::cerr << "loading config file ...";
	const auto launcher_elevation = trochia::io::config::load("config.toml", sim);
	std::cerr << std::endl;

	std::cerr << "start simulation" << std::endl;

	std::cout << "sim number: " << launcher_elevation.size() << std::endl;

	for(const auto &e : launcher_elevation){
		sim.launcher = trochia::environment::Launcher(5.0, 90.0, e);

		sim.output_dir = sim.output_dir_fmt;
		if(fs::exists(sim.output_dir)){
			if(! fs::is_directory(sim.output_dir)){
				std::cerr << "error: output dir \"" << sim.output_dir << "\""
					<< " is not directory." << std::endl;
				return -1;
			}
		}else{
			std::cerr << "output directory does not exists" << std::endl
				<< "creating \"" << sim.output_dir << "\"...";
			if(fs::create_directory(sim.output_dir))
				std::cerr << "[ok]";
			else
				std::cerr << "[failed]";
			std::cerr << std::endl;
		}
		trochia::simulation::exec(sim);
	}

	return 0;
}
