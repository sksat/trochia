#ifndef ROCKET_HPP_
#define ROCKET_HPP_

#include <memory>
#include <string>
#include "math.hpp"
#include "coordinate.hpp"

// TODO namespace

class Rocket {
public:
	std::string name;

	coordinate::local::ENU pos, vel, acc;
	math::Quaternion angle;
};

#endif
