#ifndef COORDINATE_HPP_
#define COORDINATE_HPP_

#include "math.hpp"

namespace coordinate {
	using math::Float;
	using math::Vector3, math::Matrix3, math::Quaternion;

	namespace DCM {
		inline auto ned2body(const Quaternion &q_) -> const Matrix3 {
			Matrix3 mat;
			const auto &q = q_.coeffs();
			const auto q00 = q(0) * q(0);
			const auto q11 = q(1) * q(1);
			const auto q22 = q(2) * q(2);
			const auto q33 = q(3) * q(3);
			mat <<
				q00 - q11 - q22 + q33,		2*(q(0)*q(1) + q(2)*q(3)),		2*(q(0)*q(2) - q(1)*q(3)),
				2*(q(0)*q(1) - q(2)*q(3)),	q11 - q00 - q22 + q33,			2*(q(1)*q(2) + q(0)*q(3)),
				2*(q(0)*q(2) + q(1)*q(3)),	2*(q(1)*q(2) - q(0)*q(3)),		q22 - q00 - q11 + q33;
			return mat;
		}
		inline auto body2ned(const Quaternion &q) -> const Matrix3 {
			return ned2body(q).transpose();
		}
	}

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

			static inline auto get_type() -> type { return t; }

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

			auto to_enu() -> frame<type::ENU> { return cast<type::ENU>(); }
			auto to_ned() -> frame<type::NED> { return cast<type::NED>(); }

			template<type t2>
			auto cast() const -> frame<t2> {
				return cast<frame<t2>>();
			}

			template<typename LocalFrame>
			inline auto cast() const -> LocalFrame {
				LocalFrame f;
				f.east(this->east());
				f.north(this->north());
				f.altitude(this->altitude());
				return f;
			}

			template<typename BodyFrame>
			auto to_body(const Quaternion &q) const -> BodyFrame {
				BodyFrame b;
				if constexpr(t == type::NED){
					b.vec = DCM::ned2body(q) * vec;
					return b;
				}
			}
		};
	}

	namespace body {
		class frame {
		public:
			frame() : vec(0.0, 0.0, 0.0) {}
			frame(const Float &x, const Float &y, const Float &z) : vec(x,y,z) {}

			math::Vector3 vec;

			template<typename LocalFrame>
			inline auto cast(const math::Quaternion &q) const -> const LocalFrame {
				local::frame<local::type::NED> ned;
				ned.vec = DCM::body2ned(q) * this->vec;

				return ned.cast<LocalFrame>();
			}
		};
	}
}

#endif
