#ifndef ROCKET_HPP_
#define ROCKET_HPP_

#include <string>
#include "math.hpp"

// TODO namespace

class Rocket {
public:
	std::string name;
	math::Vector3		pos, vel, acc;
	math::Quaternion	angle;
};

#endif
