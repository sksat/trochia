#include <iostream>
#include <string>
#include "rocket.hpp"
#include "environment/gravity.hpp"

using math::Float;

Rocket rocket;

auto do_simulation(const Float &dt, const Float &timeout) -> void;

auto main(int argc, char **argv) -> int {
	std::string engine_file;

	std::cout << "rocket simulator by sksat" << std::endl;

	std::cout << "engine data file: ";
	std::cin >> engine_file;

	rocket.engine.load_data(engine_file);

	std::cout << "start simulation" << std::endl;
	do_simulation(0.01, 60.0*10);

	return 0;
}

void do_simulation(const math::Float &dt, const math::Float &timeout){
	Float time = 0.0;

	// init
	rocket.mass = 10.0;

	auto &pos = rocket.pos;
	pos.altitude(0.0);
	pos.east(0.0);
	pos.north(0.0);
	rocket.vel.vec << 0.0, 0.0, 0.0;
	rocket.acc.vec << 0.0, 0.0, 0.0;

	// main loop
	for(size_t step=0;time<=timeout;time+=dt,step++){
		// thrust
		rocket.acc.altitude(rocket.engine.thrust(time) / rocket.mass);

		// gravity
		environment::gravity(rocket.pos, rocket.acc);

		// update
		rocket.update(dt);

		// TODO save to file

		// log
		if(step % 100 == 0){
			std::cout << time << " " << rocket.pos.altitude() << std::endl;
		}

		if(step >= 100 && rocket.pos.altitude() <= 0.0)
			break;
	}
}
