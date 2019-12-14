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

#ifndef ENVIRONMENT_EARTH_GEODESY_HPP_
#define ENVIRONMENT_EARTH_GEODESY_HPP_

#include "../earth.hpp"

namespace trochia::environment::earth::geodesy {
	// geopotential height
	// U.S. Standard Atmosphere, 1976: https://ntrs.nasa.gov/search.jsp?R=19770009539
	auto potential_height(const math::Float &height) -> math::Float {
		// the adopted, effective earth's radius, 6356.766km
		// used for computing g(Z) for 45-degree north latitude,
		// and used for relating H and Z at that latitude
		constexpr math::Float r0 = 6356766;
		constexpr math::Float gamma = 1.0;

		return (r0 * height) / (gamma * r0 - height);
	}
}

#endif
