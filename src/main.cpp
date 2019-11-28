#include <iostream>
#include <vector>
#include <string>
#include <toml.hpp>
#include "simulation.hpp"
#include "io/config.hpp"

using math::Float;

auto main(int argc, char **argv) -> int {
	std::vector<Simulation> sims;

	std::cout << "rocket simulator by sksat" << std::endl;

	std::cout << "loading config file ...";
	io::config::load("config.toml", sims);
	std::cout << std::endl;

	std::cout << "start simulation" << std::endl;

	std::cout << "sim num: " << sims.size() << std::endl;
	for(auto &s : sims){
		do_simulation(s);
	}

	return 0;
}
