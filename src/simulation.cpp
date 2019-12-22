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
#include <fstream>
#include <filesystem>
#include "math.hpp"
#include "simulation.hpp"
#include "solver.hpp"
#include "coordinate.hpp"
#include "environment.hpp"

namespace fs = std::filesystem;

template<size_t N=0, typename T>
auto open_file(const fs::path &dir, const T &fname, std::vector<std::ofstream> &file) -> void {
	if constexpr(N < std::tuple_size<T>::value){
		const auto &fn = std::get<N>(fname);
		file.push_back(std::ofstream(dir / fn));
		open_file<N+1>(dir, fname, file);
	}
}

auto trochia::simulation::exec(simulation::Simulation &sim) -> void {
	using math::Float;

	if(sim.output_dt < sim.dt)
		return;

	std::cout << "execute simulation case" << std::endl
		<< "\tlauncher: length=" << sim.launcher.length
		<< ", azimuth=" << sim.launcher.azimuth()
		<< ", elevation=" << sim.launcher.elevation() << std::endl;

	const size_t output_rate = sim.output_dt / sim.dt;
	auto &rocket = sim.rocket;

	// init
	rocket.time = 0.0;

	auto &pos = rocket.pos;
	pos.up(0.0);
	pos.east(0.0);
	pos.north(0.0);
	rocket.vel.vec.setZero();
	rocket.acc.vec.setZero();

	rocket.quat = sim.launcher.get_angle();

	rocket.Cmq = -1.0*rocket.Cna / 2.0 * std::pow((rocket.lcp-rocket.lcg0)/rocket.length, 2.0);

	std::pair<Float, Float> altitude_max = { 0.0, 0.0 };

	auto solve = solver::RK4(rocket, rocket::Rocket::dx);

	const auto output_fname = std::make_tuple(
		"pos.dat",
		"body.dat"
	);
	std::vector<std::ofstream> output_file;
	open_file(sim.output_dir, output_fname, output_file);

	// main loop
	size_t step = 0;
	while(true){
		do_step(sim, solve);
		step++;

		const auto &t = solve.t;
		const auto &altitude = sim.rocket.pos.altitude();

		if(altitude_max.second < altitude){
			altitude_max.first = t;
			altitude_max.second= altitude;
		}

		if(step % output_rate == 0)
			save_data(t, sim, output_file);

		// 終了判定
		if(step > 100 && altitude <= 0.0)
			break;
		if(t > sim.timeout)
			break;
	}

	std::cerr << "max altitude: " << altitude_max.second
		<< "(" << altitude_max.first << "sec)" << std::endl;
}

auto trochia::simulation::do_step(Simulation &sim, solver::solver<rocket::Rocket> &s) -> void {
	const auto &time = s.t;

	auto &rocket = sim.rocket;
	rocket.time = time;

	// 機体速度ベクトル(NED)
	const coordinate::local::NED vel_ned = rocket.vel;

	// 対気速度ベクトル
	const auto va_ned	= vel_ned.vec;
	const auto va_body	= coordinate::dcm::ned2body(rocket.quat) * va_ned;
	const auto va		= va_body.norm();

	// tan(attack) = z/x
	// sin(side_slip) = y/va
	const auto angle_attack		= (va_body.x()==0.0 ? 0.0 : std::atan(va_body.z() / va_body.x()));
	const auto angle_side_slip	= (va==0.0 ? 0.0 : std::asin(va_body.y() / va));

	// 代表面積
	const auto S = rocket.diameter * rocket.diameter * math::pi / 4;

	const auto altitude = rocket.pos.altitude();
	const auto geo_height = environment::earth::geodesy::potential_height(altitude);
	const auto temperature = environment::air::temperature(geo_height);
	const auto rho = environment::air::density(temperature);

	// 空気抵抗
	const auto q = 0.5 * rho * va * va; // 動圧
	const auto D = q * S * rocket.Cd;
	const auto Y = q * S * rocket.Cna * angle_side_slip;
	const auto N = q * S * rocket.Cna * angle_attack;

	// thrust
	const auto thrust = rocket.engine.thrust(time); // first stage only

	const auto force = coordinate::body::Body(
		thrust - D,
		-1.0 * Y,
		-1.0 * N
	);

	rocket.force(force.to_local<rocket::LocalFrame>(rocket.quat));

	// gravity
	if(rocket.pos.altitude() > 0.0){
		const auto g = environment::gravity(rocket.pos.altitude());
		rocket.acc.down(rocket.acc.down() + g);
	}

	// update
	s.step(sim.dt);
}

auto trochia::simulation::save_data(const math::Float &time, const Simulation &sim, std::vector<std::ofstream> &output) -> void {
	using std::endl;

	auto &o_pos = output[0];
	auto &o_body= output[1];

	const auto &rocket = sim.rocket;
	const auto ned2body= coordinate::dcm::ned2body(rocket.quat);

	const auto &pos = rocket.pos;
	const auto b_vel= ned2body * rocket.vel.vec;
	const auto b_acc= ned2body * rocket.acc.vec;

	o_pos << time << " "
		<< pos.east() << " " << pos.up() << " " << pos.north() << " " << endl;

	o_body << time << " "
		<< b_vel.x() << " " << b_vel.y() << " " << b_vel.z() << " "
		<< b_acc.x() << " " << b_acc.y() << " " << b_acc.z() << endl;
}
