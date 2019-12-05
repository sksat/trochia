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
