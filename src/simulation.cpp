#include <iostream>
#include "math.hpp"
#include "simulation.hpp"
#include "solver.hpp"
#include "environment/launcher.hpp"
#include "environment/gravity.hpp"

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

	auto launcher = environment::Launcher(5.0);	// 5.0mのランチャ

	// TODO: set launcher position
	launcher.azimuth(90.0);		// 方位角(deg)
	launcher.elevation(80.0);	// 仰角(deg)

	auto &pos = rocket.pos;
	pos.up(0.0);
	pos.east(0.0);
	pos.north(0.0);
	rocket.vel.vec.setZero();
	rocket.acc.vec.setZero();

	rocket.quat = launcher.get_angle();

	auto s = solver::RK4(rocket, rocket::Rocket::dx);

	// main loop
	size_t step = 0;
	while(true){
		const auto &time = s.t;
		rocket.time = time;

		// thrust
		const auto thrust = rocket.engine.thrust(time); // first stage only

		const auto force = coordinate::body::Body(
			thrust,
			0.0,
			0.0
		);

		rocket.force(force.to_local<rocket::LocalFrame>(rocket.quat));

		// gravity
		if(rocket.pos.altitude() > 0.0)
			environment::gravity(rocket.pos, rocket.acc);

		// update
		s.step(dt);
		step++;

		// TODO save to file

		// log
		if(step % output_rate == 0){
			std::cout << time << " "
				<< rocket.pos.altitude() << " "
				<< rocket.lcg() << std::endl;
		}

		if(step > 100 && rocket.pos.altitude() <= 0.0)
			break;

		if(time > timeout)
			break;
	}
}
