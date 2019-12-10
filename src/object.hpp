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

#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "math.hpp"

namespace trochia::object {
	template<typename Frame>
	class Object {
	public:
		Object() {}
		Object(const Frame pos) : pos(pos) {}

		math::Float time;
		Frame pos, vel, acc;
		math::Quaternion quat;

		static auto dx(const math::Float &t, const Object &x) -> const Object {
			Object ret;
			ret.pos = x.vel;
			ret.vel = x.acc;
			return ret;
		}

		virtual auto weight() const -> const math::Float {
			return 0.0;
		}

		inline auto force(const Frame &f) -> void {
			this->acc = f;
			this->acc /= this->weight();
		}

		// operator
		auto operator+=(const Object &o) -> const Object {
			this->pos += o.pos;
			this->vel += o.vel;
			return *this;
		}
		auto operator-=(const Object &o) -> const Object {
			this->pos -= o.pos;
			this->vel -= o.vel;
			return *this;
		}

		auto operator*=(const math::Float &a) -> const Object {
			this->pos *= a;
			this->vel *= a;
			return *this;
		}
		auto operator/=(const math::Float &a) -> const Object {
			this->pos /= a;
			this->vel /= a;
			return *this;
		}

		auto operator+(const Object &o) const -> const Object {
			Object ret = *this;
			ret += o;
			return ret;
		}
		auto operator-(const Object &o) const -> const Object {
			Object ret = *this;
			ret -= o;
			return ret;
		}

		auto operator*(const math::Float &a) const -> const Object {
			Object ret = *this;
			ret *= a;
			return ret;
		}
		auto operator/(const math::Float &a) const -> const Object {
			Object ret = *this;
			ret /= a;
			return ret;
		}
	};
}

#endif
