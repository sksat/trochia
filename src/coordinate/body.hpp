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

#ifndef COORDINATE_BODY_HPP_
#define COORDINATE_BODY_HPP_

namespace trochia::coordinate::body {
	class Body {
	public:
		Body() : vec(0.0, 0.0, 0.0) {}
		Body(const Float &x, const Float &y, const Float &z) : vec(x,y,z) {}

		Vector3 vec;

		template<typename LocalFrame>
		inline auto to_local(const Quaternion &q) const -> const LocalFrame {
			local::NED ned;
			ned.vec = dcm::body2ned(q) * this->vec;

			return ned;
		}
	};
}

#endif
