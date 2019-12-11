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

#ifndef ENVIRONMENT_GRAVITY_HPP_
#define ENVIRONMENT_GRAVITY_HPP_

#include "../math.hpp"

namespace trochia::environment {
	namespace constant {
		// 2018 CODATA recommended values
		constexpr math::Float G		= 6.6743015e-11;

		// standard acceleration of gravity(definition)
		constexpr math::Float gn	= 9.80665;

		// Chronological Scientific Tables 2014
		constexpr math::Float re	= 6.3781366e6;
	}

	// TODO
	// latitude model(Helmert equation, WGS-84)
	// gravitation model
	// other planet

	inline auto gravity(const math::Float &altitude) -> const math::Float {
		const auto tmp = constant::re / (constant::re + altitude);
		return constant::gn * tmp * tmp;
	}
}

#endif
