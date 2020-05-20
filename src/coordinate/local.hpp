/* ----------------------------------------------------------------------- *
 *
 *    Copyright (C) 2019-2020 sksat <sksat@sksat.net>
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

#ifndef COORDINATE_LOCAL_HPP_
#define COORDINATE_LOCAL_HPP_

namespace trochia::coordinate::local {
	class Base {
	public:
		Base() : vec(0.0, 0.0, 0.0){}
		Base(math::Float x, math::Float y, math::Float z) : vec(x, y, z) {}

		Vector3 vec;

		inline auto operator+(const Base &b) const -> Base {
			Base ret;
			ret.vec = this->vec + b.vec;
			return ret;
		}
		inline auto operator-(const Base &b) const -> Base {
			Base ret;
			ret.vec = this->vec - b.vec;
			return ret;
		}

		inline auto operator*(const math::Float &b) const -> Base {
			Base ret;
			ret.vec = this->vec * b;
			return ret;
		}
		inline auto operator/(const math::Float &b) const -> Base {
			Base ret;
			ret.vec = this->vec / b;
			return ret;
		}

		inline auto operator+=(const Base &b) -> Base& {
			this->vec += b.vec;
			return *this;
		}
		inline auto operator-=(const Base &b) -> Base& {
			this->vec -= b.vec;
			return *this;
		}

		inline auto operator*=(const math::Float &b) -> Base& {
			this->vec *= b;
			return *this;
		}
		inline auto operator/=(const math::Float &b) -> Base& {
			this->vec /= b;
			return *this;
		}
	};

	class NED : public Base {
	public:
		NED() : Base() {}
		NED(math::Float x, math::Float y, math::Float z) : Base(x,y,z) {}

		auto north() const -> Float { return vec.x(); }
		auto east()  const -> Float { return vec.y(); }
		auto down()  const -> Float { return vec.z(); }

		auto south() const -> Float { return -1.0*north(); }
		auto west()  const -> Float { return -1.0*east();  }
		auto up()    const -> Float { return -1.0*down();  }

		auto altitude() const -> Float { return up(); }

		auto north(const Float &n) -> void { vec.x() = n; }
		auto east(const Float &e)  -> void { vec.y() = e; }
		auto down(const Float &d)  -> void { vec.z() = d; }

		auto south(const Float &s) -> void { north(-1.0*s); }
		auto west(const Float &w)  -> void { east(-1.0*w);  }
		auto up(const Float &u)    -> void { down(-1.0*u);  }

		template<typename Frame>
		operator Frame() const {
			Frame frame;
			frame.north(this->north());
			frame.east(this->east());
			frame.down(this->down());
			return frame;
		}
	};

	class ENU : public Base {
	public:
		ENU() : Base() {}
		ENU(math::Float x, math::Float y, math::Float z) : Base(x,y,z) {}

		auto east()  const -> Float { return vec.x(); }
		auto north() const -> Float { return vec.y(); }
		auto up()    const -> Float { return vec.z(); }

		auto west()  const -> Float { return -1.0*east();  }
		auto south() const -> Float { return -1.0*north(); }
		auto down()  const -> Float { return -1.0*up();    }

		auto altitude() const -> Float { return up(); }

		auto east(const Float &e)  -> void { vec.x() = e; }
		auto north(const Float &n) -> void { vec.y() = n; }
		auto up(const Float &u)    -> void { vec.z() = u; }

		auto west(const Float &w)  -> void { east(-1.0*w); }
		auto south(const Float &s) -> void { north(-1.0*s);}
		auto down(const Float &d)  -> void { up(-1.0*d);   }

		template<typename Frame>
		operator Frame() const {
			Frame frame;
			frame.east(this->east());
			frame.north(this->north());
			frame.up(this->up());
			return frame;
		}
	};
}

#endif
