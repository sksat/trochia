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

#ifndef ENVIRONMENT_WIND_HPP_
#define ENVIRONMENT_WIND_HPP_

#include "../math.hpp"

namespace trochia::environment::wind {
	using math::Float;
	// 上空風速(べき法則)
	inline auto speed(const Float n, const Float &z_r, const Float v_r, const Float altitude) -> math::Float {
		const auto tmp = altitude / z_r;
		return v_r * std::pow(tmp, Float(1.0) / n);
	}
}

#endif
