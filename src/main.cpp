#include <iostream>
#include <string>
#include <toml.hpp>
#include "rocket.hpp"
#include "environment/gravity.hpp"

using math::Float;

Rocket rocket;

auto do_simulation(const Float &dt, const Float &output_dt, const Float &timeout) -> void;

auto main(int argc, char **argv) -> int {
	math::Float dt, output_dt;
	std::string engine_file;

	std::cout << "rocket simulator by sksat" << std::endl;

	std::cout << "loading config file ...";
	{
		using namespace toml;
		const auto config = parse("config.toml");
		const auto &simulation = find(config, "simulation");
		dt = simulation.at("dt").as_floating();
		const auto &output = find(simulation, "output");
		output_dt = output.at("dt").as_floating();

		const auto &rocket = find(config, "rocket");
		const auto stage = find<std::vector<toml::table>>(rocket, "stage");
		if(stage.size() != 1){
			std::cout << "multistage rocket is not implemented now. sorry." << std::endl;
			return -1;
		}
		const auto engine = stage[0].at("engine");
		engine_file = engine.as_string();
	}

	rocket.engine.load_eng(engine_file);

	std::cout << "start simulation" << std::endl;
	do_simulation(dt, output_dt, 60.0*10);

	return 0;
}

void do_simulation(const Float &dt, const Float &output_dt, const Float &timeout){
	if(output_dt < dt)
		return;

	const size_t output_rate = output_dt / dt;

	// init
	rocket.time = 0.0;
	auto &pos = rocket.pos;
	pos.altitude(0.0);
	pos.east(0.0);
	pos.north(0.0);
	rocket.vel.vec << 0.0, 0.0, 0.0;
	rocket.acc.vec << 0.0, 0.0, 0.0;

	// main loop
	size_t step = 0;
	while(true){
		// thrust
		const auto thrust = rocket.engine.thrust(rocket.time); // first stage only
		rocket.acc.altitude(thrust / rocket.weight());

		// gravity
		if(rocket.pos.altitude() > 0.0)
			environment::gravity(rocket.pos, rocket.acc);

		// update
		rocket.update(dt);
		step++;

		// TODO save to file

		// log
		if(step % output_rate == 0){
			std::cout << rocket.time << " " << rocket.pos.altitude() << std::endl;
		}

		if(step > 100 && rocket.pos.altitude() <= 0.0)
			break;

		if(rocket.time > timeout)
			break;
	}
}
