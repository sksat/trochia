#ifndef COORDINATE_HPP_
#define COORDINATE_HPP_

#include <optional>
#include "math.hpp"

namespace coordinate {
	using math::Float;
	using math::Vector3, math::Matrix3, math::Quaternion;

	using optFloat = std::optional<Float>;

	namespace local {
		class NED;
		class ENU;
	}

	namespace body {
		class body;
	}
}

#include "coordinate/dcm.hpp"
#include "coordinate/local.hpp"
#include "coordinate/body.hpp"

#endif
