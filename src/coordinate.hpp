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

#ifndef COORDINATE_HPP_
#define COORDINATE_HPP_

#include <optional>
#include "math.hpp"

namespace trochia::coordinate {
	using math::Float;
	using math::Vector3, math::Matrix3, math::Quaternion;

	using optFloat = std::optional<Float>;

	namespace earth {
		class LLH;
		class ECEF;
		class ECI;
	}

	namespace local {
		class NED;
		class ENU;
	}

	namespace body {
		class body;
	}
}

#include "coordinate/dcm.hpp"
#include "coordinate/earth.hpp"
#include "coordinate/local.hpp"
#include "coordinate/body.hpp"

#endif
