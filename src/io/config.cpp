#include <iostream>
#include <toml.hpp>
#include "../simulation.hpp"
#include "config.hpp"

namespace io::config {

auto load(const std::string &fname, std::vector<Simulation> &sims) -> void {
	using namespace toml;

	Simulation sim;

	const auto config = parse(fname);

	// simulation common
	{
		const auto &cfg_sim = find(config, "simulation");
		sim.dt = cfg_sim.at("dt").as_floating();

		const auto output = find(cfg_sim, "output");
		sim.output_dt = output.at("dt").as_floating();
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

		sim.rocket.lcg0 = stage[0].at("lcg0").as_floating();
		sim.rocket.lcgf = stage[0].at("lcgf").as_floating();
	}

	sims.push_back(sim);
}

}
