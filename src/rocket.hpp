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
	using LocalFrame = coordinate::local::NED;

	std::string name;
	Engine engine;

	math::Float time;			// from ignition
	LocalFrame pos, vel, acc;
	math::Quaternion quat;

	auto weight() const  -> const math::Float {
		return 10.0 + engine.weight(time);
	}

	// update(euler method)
	auto update(const math::Float &dt) -> void {
		vel += acc * dt;
		pos += vel * dt;
		time += dt;
	}
};

#endif
