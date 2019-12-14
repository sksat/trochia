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
#include "math.hpp"
#include "simulation.hpp"
#include "solver.hpp"
#include "environment.hpp"

auto trochia::do_simulation(Simulation &sim) -> void {
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
		if(rocket.pos.altitude() > 0.0){
			const auto g = environment::gravity(rocket.pos.altitude());
			rocket.acc.down(rocket.acc.down() + g);
		}

		// update
		s.step(dt);
		step++;

		// TODO save to file

		// log
		if(step % output_rate == 0){
			const auto altitude		= rocket.pos.altitude();
			const auto geo_height	= environment::earth::geodesy::potential_height(altitude);
			const auto temperature = environment::air::temperature(geo_height);

			std::cout << time << " "
				<< altitude << " "
				<< geo_height << " "
				<< (math::Float)environment::temperature::celsius(temperature)
				<< std::endl;
		}

		if(step > 100 && rocket.pos.altitude() <= 0.0)
			break;

		if(time > timeout)
			break;
	}
}
