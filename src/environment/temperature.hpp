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

#ifndef ENVIRONMENT_TEMPERATURE_HPP_
#define ENVIRONMENT_TEMPERATURE_HPP_

#include "../math.hpp"

namespace trochia::environment::temperature {
	class thermodynamic {
	public:
		constexpr thermodynamic() : t(0.0) {}
		constexpr thermodynamic(const math::Float &t) : t(t) {}

		virtual operator math::Float() {
			return t;
		}
	protected:
		math::Float t;
	};

	using kelvin = thermodynamic;

	class celsius : public thermodynamic {
	public:
		constexpr celsius() : thermodynamic(273.15) {}
		constexpr celsius(const thermodynamic &t) : thermodynamic(t) {}
		constexpr celsius(const math::Float &t)   : thermodynamic(t + 273.15) {}

		operator math::Float() {
			return this->t - 273.15;
		}
	};
}

#endif
