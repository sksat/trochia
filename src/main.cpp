#include <iostream>
#include <vector>
#include <string>
#include <toml.hpp>
#include "simulation.hpp"
#include "io/config.hpp"

auto main(int argc, char **argv) -> int {
	std::vector<trochia::Simulation> sims;

	std::cerr << "rocket simulator by sksat" << std::endl;

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
