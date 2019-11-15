#include <iostream>
#include <toml.hpp>
#include "../simulation.hpp"
#include "config.hpp"

auto io::config::load(const std::string &fname, std::vector<Simulation> &sims) -> void {
	using namespace toml;

	Simulation sim;

	const auto config = parse(fname);

	const auto &cfg_sim = find(config, "simulation");
	sim.dt = cfg_sim.at("dt").as_floating();

	const auto &cfg_output = find(cfg_sim, "output");
	sim.output_dt = cfg_output.at("dt").as_floating();

	const auto &rocket = find(config, "rocket");
	const auto stage = find<std::vector<toml::table>>(rocket, "stage");
	if(stage.size() != 1){
		std::cerr
			<< "multistage rocket is not implemented now. sorry."
			<< std::endl;
	}

	const auto engine = stage[0].at("engine");
	sim.rocket.engine.load_eng(engine.as_string());

	sims.push_back(sim);
}
