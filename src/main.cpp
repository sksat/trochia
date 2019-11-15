#include <iostream>
#include <vector>
#include <string>
#include <toml.hpp>
#include "simulation.hpp"

#include "rocket.hpp"
#include "io/config.hpp"
#include "environment/launcher.hpp"
#include "environment/gravity.hpp"

using math::Float;

auto do_simulation(Simulation &sim) -> void;

auto main(int argc, char **argv) -> int {
	std::vector<Simulation> sims;

	std::cout << "rocket simulator by sksat" << std::endl;

	std::cout << "loading config file ...";
	io::config::load("config.toml", sims);
	std::cout << std::endl;

	std::cout << "start simulation" << std::endl;

	std::cout << sims.size() << std::endl;
	for(auto &s : sims){
		do_simulation(s);
	}

	return 0;
}

auto do_simulation(Simulation &sim) -> void {
	const auto &timeout = sim.timeout;
	const auto &dt = sim.dt;
	const auto &output_dt = sim.output_dt;
	if(output_dt < dt)
		return;

	const size_t output_rate = sim.output_dt / sim.dt;
	auto &rocket = sim.rocket;

	// init
	rocket.time = 0.0;

	const auto launcher = environment::Launcher(5.0, 90.0, 80.0);

	auto &pos = rocket.pos;
	pos.up(0.0);
	pos.east(0.0);
	pos.north(0.0);
	rocket.vel.vec.setZero();
	rocket.acc.vec.setZero();

	rocket.quat = launcher.get_quat();

	// main loop
	size_t step = 0;
	while(true){
		// thrust
		const auto thrust = rocket.engine.thrust(rocket.time); // first stage only

		const auto force = coordinate::body::Body(
			thrust,
			0.0,
			0.0
		);

		auto acc = force;
		acc.vec /= rocket.weight();

		rocket.acc = acc.to_local<Rocket::LocalFrame>(rocket.quat);

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
