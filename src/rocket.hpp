#ifndef ROCKET_HPP_
#define ROCKET_HPP_

#include <memory>
#include <string>
#include "math.hpp"
#include "coordinate.hpp"
#include "engine.hpp"

// TODO namespace

class Rocket {
public:
	using LocalFrame = coordinate::local::frame<coordinate::local::type::ENU>;

	std::string name;
	Engine engine;

	math::Float mass;

	LocalFrame pos, vel, acc;
	math::Quaternion angle;

	// update(euler method)
	auto update(const math::Float &dt) -> void {
		vel.vec += acc.vec * dt;
		pos.vec += vel.vec * dt;
	}
};

#endif
