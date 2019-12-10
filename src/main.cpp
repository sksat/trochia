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
#include <toml.hpp>
#include "simulation.hpp"
#include "io/config.hpp"

auto main(int argc, char **argv) -> int {
	std::vector<trochia::Simulation> sims;

	std::cerr << "Trochia: rocket simulator by sksat" << std::endl;

	std::cerr
		<< R"(
    Trochia  Copyright (C) 2019  sksat <sksat@sksat.net>
    This program comes with ABSOLUTELY NO WARRANTY; for details execute 'trochia --version'
    This is free software, and you are welcome to redistribute it
    under certain conditions; execute 'trochia --version' for details.

)";

	std::cerr << "loading config file ...";
	trochia::io::config::load("config.toml", sims);
	std::cerr << std::endl;

	std::cerr << "start simulation" << std::endl;

	std::cerr << "sim num: " << sims.size() << std::endl;
	for(auto &s : sims){
		do_simulation(s);
	}

	return 0;
}
