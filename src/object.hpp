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
		math::Vector3 omega, domega;

		static auto dx(const math::Float &t, const Object &x) -> const Object {
			Object ret;
			ret.pos = x.vel;
			ret.vel = x.acc;

			ret.omega = x.domega;

			// http://www.mss.co.jp/technology/report/pdf/18-07.pdf 4.3.2
			const auto &omg = x.omega;
			math::Matrix4 mat;
			mat <<
				0.0,			omg(2),			-1.0*omg(1),	omg(0),
				-1.0*omg(2),	0.0,			omg(0),			omg(1),
				omg(1),			-1.0*omg(0),	0.0,			omg(2),
				-1.0*omg(0),	-1.0*omg(1),	-1.0*omg(2),	0.0;

			math::Vector4 dq = mat * math::quat2vec(x.quat);

			ret.quat = math::vec2quat(dq);

			return ret;
		}

		virtual auto weight() const -> math::Float {
			return 0.0;
		}

		inline auto force(const Frame &f) -> void {
			this->acc = f;
			this->acc /= this->weight();
		}

		// operator
		auto operator+=(const Object &o) -> Object {
			this->pos += o.pos;
			this->vel += o.vel;

			this->omega += o.omega;

			auto q  = math::quat2vec(this->quat);
			auto oq = math::quat2vec(o.quat);

			q += oq;
			this->quat = math::vec2quat(q);

			return *this;
		}
		auto operator-=(const Object &o) -> Object {
			this->pos -= o.pos;
			this->vel -= o.vel;

			this->omega -= o.omega;

			auto q  = math::quat2vec(this->quat);
			auto oq = math::quat2vec(o.quat);

			q -= oq;

			this->quat = math::vec2quat(q);

			return *this;
		}

		auto operator*=(const math::Float &a) -> Object {
			this->pos *= a;
			this->vel *= a;

			this->omega *= a;

			auto q = math::quat2vec(this->quat);
			q *= a;
			this->quat = math::vec2quat(q);

			return *this;
		}
		auto operator/=(const math::Float &a) -> Object {
			this->pos /= a;
			this->vel /= a;

			this->omega /= a;

			auto q = math::quat2vec(this->quat);
			q /= a;
			this->quat = math::vec2quat(q);

			return *this;
		}

		auto operator+(const Object &o) const -> Object {
			Object ret = *this;
			ret += o;
			return ret;
		}
		auto operator-(const Object &o) const -> Object {
			Object ret = *this;
			ret -= o;
			return ret;
		}

		auto operator*(const math::Float &a) const -> Object {
			Object ret = *this;
			ret *= a;
			return ret;
		}
		auto operator/(const math::Float &a) const -> Object {
			Object ret = *this;
			ret /= a;
			return ret;
		}
	};
}

#endif
