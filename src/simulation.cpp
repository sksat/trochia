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

auto is_launch_clear(const trochia::environment::Launcher &launcher, const trochia::coordinate::local::NED &ned) -> bool {
	return ned.vec.norm() > launcher.length;
}

auto trochia::simulation::exec(simulation::Simulation &sim) -> void {
	using math::Float;

	if(sim.output_dt < sim.dt)
		return;

	std::cout << "execute simulation case" << std::endl
		<< "\tlauncher: length=" << sim.launcher.length
			<< ", azimuth=" << sim.launcher.azimuth()
			<< ", elevation=" << sim.launcher.elevation() << std::endl
		<< "\twind: dir=" << sim.wind_dir
			<< ", speed=" << sim.wind_speed << std::endl;

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
	sim.launch_clear = { 0.0, 0.0 };

	auto solve = solver::RK4(rocket, rocket::Rocket::dx);

	const auto output_fname = std::make_tuple(
		"pos.dat",
		"body.dat",
		"angle.dat",
		"force.dat"
	);
	std::vector<std::ofstream> output_file;
	open_file(sim.output_dir, output_fname, output_file);

	// main loop
	size_t step = 0;
	while(true){
		do_step(sim, solve);
		step++;

		const auto &t = solve.t;

		const coordinate::local::NED pos_ned = sim.rocket.pos;
		const auto &altitude = pos_ned.altitude();

		if(sim.launch_clear.first == 0.0){
			if(is_launch_clear(sim.launcher, pos_ned)){
				sim.launch_clear.first	= t;
				sim.launch_clear.second	= sim.rocket.vel.vec.norm();
			}
		}

		if(altitude_max.second < altitude){
			altitude_max.first = t;
			altitude_max.second= altitude;
		}

		if(step % output_rate == 0)
			save_data(t, sim, output_file);

		// end simulation
		if(is_launch_clear(sim.launcher, sim.rocket.pos) && altitude <= 0.0)
			break;
		if(t > sim.timeout)
			break;
	}

	std::cout
		<< "\tlaunch clear: " << sim.launch_clear.second << "m/s"
			<< " (" << sim.launch_clear.first << "s)" << std::endl
		<< "\tmax altitude: " << altitude_max.second << "m"
			<< " (" << altitude_max.first << "s)" << std::endl
		<< "\tGHP: (" << sim.rocket.pos.east() << ", " << sim.rocket.pos.north() << ")" << std::endl;
}

auto trochia::simulation::do_step(Simulation &sim, solver::solver<rocket::Rocket> &s) -> void {
	const auto &time = s.t;

	auto &rocket = sim.rocket;
	rocket.time = time;

	// rocket speed vector(NED)
	const coordinate::local::NED vel_ned = rocket.vel;

	// wind speed vector(NED)
	const auto wind_vel	= environment::wind::speed(6.0, 2.0, sim.wind_speed, rocket.pos.altitude());
	const auto wind_dir_rad = math::deg2rad(sim.wind_dir);
	const auto wind_ned = math::Vector3(
			wind_vel * std::cos(wind_dir_rad),
			wind_vel * std::sin(wind_dir_rad),
			0.0);

	// airspeed vector(NED, body)
	const auto va_ned	= vel_ned.vec - wind_ned;
	const auto va_body	= coordinate::dcm::ned2body(rocket.quat) * va_ned;
	const auto va		= va_body.norm();

	// tan(attack) = z/x
	// sin(side_slip) = y/va
	rocket.angle_attack		= (va_body.x()==0.0 ? 0.0 : std::atan(va_body.z() / va_body.x()));
	rocket.angle_side_slip	= (va==0.0 ? 0.0 : std::asin(va_body.y() / va));

	const auto S = rocket.diameter * rocket.diameter * math::pi / 4;

	const auto altitude = rocket.pos.altitude();
	const auto geo_height = environment::earth::geodesy::potential_height(altitude);
	const auto temperature = environment::air::temperature(geo_height);
	const auto rho = environment::air::density(temperature);

	// Air resistance
	const auto q = 0.5 * rho * va * va;							// dynamic pressure
	rocket.D = q * S * rocket.Cd;							// Drag Force
	rocket.N = q * S * rocket.Cna * rocket.angle_attack;	// Normal Force
	rocket.Y = q * S * rocket.Cna * rocket.angle_side_slip;	// Normal Force on Y-axis

	// thrust
	const auto thrust = rocket.engine.thrust(time); // first stage only

	const auto force = coordinate::body::Body(
		thrust - rocket.D,
		-1.0 * rocket.Y,
		-1.0 * rocket.N
	);

	rocket.force(force.to_local<rocket::LocalFrame>(rocket.quat));

	// gravity
	if(rocket.pos.altitude() > 0.0){
		const auto g = environment::gravity(rocket.pos.altitude());
		rocket.acc.down(rocket.acc.down() + g);
	}

	// Coefficient of Damping moment by air force
	const auto Ka_div = 2.0 * va * rocket.Cmq;
	const auto Ka = (Ka_div==0.0 ? 0.0 :
			q * S * rocket.length * rocket.length / Ka_div);

	// Coefficient of Jet damping
	const auto mm0		= 2.476;	// TODO: engine.weight(0.0)
	const auto Ip0		= (rocket.lcgp-rocket.lcg0)*(rocket.lcgp-rocket.lcg0)*mm0;
	const auto mp0		= rocket.engine.weight(0.0) - rocket.engine.weight(rocket.engine.time_end);
	const auto lcg_lcgp	= rocket.lcg() - rocket.lcgp;
	const auto l_lcg	= rocket.length - rocket.lcg();
	const auto mdot		= rocket.engine.weight(time) - rocket.engine.weight(time+sim.dt);
	const auto Kj = -(Ip0/mp0 + lcg_lcgp*lcg_lcgp - l_lcg*l_lcg)*mdot;

	// rotation
	const auto lcg_lcp = rocket.lcg() - rocket.lcp;					// length of CG~CP
	const auto ma_y = -1.0*lcg_lcp*rocket.Y + (Ka+Kj)*rocket.omega.y();	// Aerodynamic moment around Y axis
	const auto ma_z =      lcg_lcp*rocket.N + (Ka+Kj)*rocket.omega.z();	// Aerodynamic moment around Z axis

	const auto I = rocket.inertia();				// Moment of Inertia

	// Angular Acceleration
	const auto domg_y = ma_y / I;
	const auto domg_z = ma_z / I;

	// update omega
	rocket.omega.y() += domg_y * sim.dt;
	rocket.omega.z() += domg_z * sim.dt;

	// update
	s.step(sim.dt);
}

auto trochia::simulation::save_data(const math::Float &time, const Simulation &sim, std::vector<std::ofstream> &output) -> void {
	using std::endl;

	auto &o_pos		= output[0];
	auto &o_body	= output[1];
	auto &o_angle	= output[2];
	auto &o_force	= output[3];

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

	o_angle << time << " "
		<< rocket.angle_attack << " " << rocket.angle_side_slip << endl;

	o_force << time << " "
		<< rocket.D << " " << rocket.N << " " << rocket.Y << endl;
}
