#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

#include "math.hpp"
#include "rocket.hpp"

class Simulation {
public:
	math::Float timeout = 60.0 * 10;
	math::Float dt, output_dt;

	rocket::Rocket rocket;
};

auto do_simulation(Simulation &sim) -> void;

#endif
