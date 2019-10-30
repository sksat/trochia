#ifndef COORDINATE_HPP_
#define COORDINATE_HPP_

#include "math.hpp"

namespace coordinate {
	using math::Float;

	namespace earth {
		// TODO: LLH, ECEF
	}

	namespace local {
		enum class type {
			ENU,
			NED,
		};

		class base : public math::Vector3 {
		public:
			virtual auto altitude(const Float v=math::nan) -> Float = 0;
			virtual auto north(const Float v=math::nan)    -> Float = 0;
			virtual auto east(const Float v=math::nan)     -> Float = 0;

			auto south(const Float v=math::nan) -> Float {
				if(!std::isnan(v)) north(-1.0 * v);
				return -1.0 * north();
			}
			auto west(const Float v=math::nan) -> Float {
				if(!std::isnan(v)) east(-1.0 * v);
				return -1.0 * west();
			}
		};

		template<typename Src, typename Dest>
		auto convert(const Src &src) -> const Dest {
			Dest d;
			d.altitude(src.altitude());
			d.north(src.north());
			d.east(src.east());
			return d;
		}

		// East North Up
		class ENU : public base {
		public:
			auto east(const Float v=math::nan) -> Float {
				if(!std::isnan(v)) x() = v;
				return x();
			}

			auto north(const Float v=math::nan) -> Float {
				if(!std::isnan(v)) y() = v;
				return y();
			}

			auto altitude(const Float v=math::nan) -> Float {
				if(!std::isnan(v)) z() = v;
				return z();
			}
		};

		// North East Down
		class NED : public base {
		public:
			auto north(const Float v=math::nan) -> Float {
				if(!std::isnan(v)) x() = v;
				return x();
			}

			auto east(const Float v=math::nan) -> Float {
				if(!std::isnan(v)) y() = v;
				return y();
			}

			auto altitude(const Float v=math::nan) -> Float {
				if(!std::isnan(v)) z() = -1.0 * v;
				return -1.0 * z();
			}
		};
	}

	namespace body {
		// TODO: body, wind
	}
}

#endif
