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

#ifndef ENVIRONMENT_PHYSICS_HPP_
#define ENVIRONMENT_PHYSICS_HPP_

#include "../math.hpp"

namespace trochia::environment::physics {
	using math::Float;

	// standard acceleration of gravity(definition value)
	// The third General Conference on Weights and Mesures(1901)
	constexpr Float g0 = 9.80665;

	inline namespace CODATA2018 {
		// 2018 CODATA recommended values
		// https://physics.nist.gov/cuu/pdf/wall_2018.pdf

		// speed of light in vacuum
		constexpr Float c	= 299792458;

		// Newtonian constant of gravitation
		constexpr Float G	= 6.6743015e-11;
		
		// molar gas constant
		constexpr Float R	= 8.314462618;
	}

	namespace CODATA2014 {
		// 2014 CODATA recommended values
		// https://physics.nist.gov/cuu/pdf/wallet_2014.pdf

		constexpr Float c	= 299792458;
		constexpr Float G	= 6.6740831e-11;
		constexpr Float R	= 8.314459848;
	}

	namespace CODATA2010 {
		// 2010 CODATA recommended values
		// https://physics.nist.gov/cuu/pdf/all_2010.pdf

		constexpr Float c	= 299792458;
		constexpr Float G	= 6.6738480e-11;
		constexpr Float R	= 8.314462175;
	}
}

#endif
