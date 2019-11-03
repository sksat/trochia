#ifndef COORDINATE_HPP_
#define COORDINATE_HPP_

#include "math.hpp"

namespace coordinate {
	using math::Float;

	namespace earth {
		enum class type {
			LLH,
			ECEF,
		};

		template<type t=type::LLH>
		class frame {};
	}

	namespace local {
		enum class type {
			ENU,			// x = east,	y = north,	z = altitude
			NED,			// x = north,	y = east,	z = -1.0 * altitude
		};

		template<type t>
		class frame {
		public:
			frame() : vec(0.0, 0.0, 0.0) {}

			math::Vector3 vec;

			auto east() const -> Float {
				if constexpr (t == type::ENU)		return vec.x();
				else if constexpr (t == type::NED)	return vec.y();
			}
			auto east(const Float &e) -> void {
				if constexpr (t == type::ENU)		vec.x() = e;
				else if constexpr (t == type::NED)	vec.y() = e;
			}

			auto north() const -> Float {
				if constexpr (t == type::ENU)		return vec.y();
				else if constexpr (t == type::NED)	return vec.x();
			}
			auto north(const Float &n) -> void {
				if constexpr (t == type::ENU)		vec.y() = n;
				else if constexpr (t == type::NED)	vec.x() = n;
			}
			
			auto altitude() const -> Float {
				if constexpr (t == type::ENU)		return vec.z();
				else if constexpr (t == type::NED)	return vec.z() * -1.0;
			}
			auto altitude(const Float &a) -> void {
				if constexpr (t == type::ENU)		vec.z() = a;
				else if constexpr (t == type::NED)	vec.z() = a * -1.0;
			}

			auto west() -> Float { return -1.0 * east(); }
			auto west(const Float &w){ east(-1.0 * w); }

			auto south() -> Float { return -1.0 * north(); }
			auto south(const Float &s) { north(-1.0 * s); }

			template<type t2>
			auto cast() -> frame<t2> {
				frame<t2> f;
				f.east(this->east());
				f.north(this->north());
				f.altitude(this->altitude());
				return f;
			}
		};
	}

	namespace body {
		// TODO: body, wind
	}
}

#endif
