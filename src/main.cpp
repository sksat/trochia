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
	std::vector<trochia::Simulation> sims;

	trochia::version::version();

	std::cerr << "loading config file ...";
	trochia::io::config::load("config.toml", sims);
	std::cerr << std::endl;

	std::cerr << "start simulation" << std::endl;

	std::cerr << "sim num: " << sims.size() << std::endl;
	for(auto &s : sims){
		s.output_dir = s.output_dir_fmt;

		if(fs::exists(s.output_dir)){
			if(! fs::is_directory(s.output_dir)){
				std::cerr << "error: output dir \"" << s.output_dir << "\""
					<< " is not directory." << std::endl;
				return -1;
			}
		}else{
			std::cerr << "output directory does not exists" << std::endl
				<< "creating \"" << s.output_dir << "\"...";
			if(fs::create_directory(s.output_dir))
				std::cerr << "[ok]";
			else
				std::cerr << "[failed]";
			std::cerr << std::endl;
		}
		do_simulation(s);
	}

	return 0;
}
