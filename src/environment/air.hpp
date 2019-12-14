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

#ifndef ENVIRONMENT_AIR_HPP_
#define ENVIRONMENT_AIR_HPP_

#include "../math.hpp"
#include "temperature.hpp"

namespace trochia::environment::air {
	// from FROGS (return kelvin)
	auto temperature(const math::Float &height) -> temperature::kelvin {
		const auto height_km = height * 0.001;
		return temperature::celsius(15.0 - 6.5*height_km);
	}

	// from FROGS (return Pa)
	auto pressure(const temperature::kelvin &t) -> math::Float {
		const math::Float t_ = t;
		return 101325.0 * std::pow((288.15 / t_), -5.256);
	}

	// from FROGS (Don't use this equation over 11km)
	auto density(const temperature::kelvin &t) -> math::Float {
		const math::Float t_ = t;
		return (0.0034837 * pressure(t)) / t_;
	}
}

#endif
