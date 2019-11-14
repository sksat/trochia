#ifndef ENVIRONMENT_LAUNCHER_HPP_
#define ENVIRONMENT_LAUNCHER_HPP_

#include "../math.hpp"

namespace environment {
	using math::Float;

	class Launcher {
	public:
		constexpr Launcher(Float length, Float azimuth, Float elevation) : length(length), azimuth(azimuth), elevation(elevation) {}

		auto get_quat() const -> math::Quaternion {
			using namespace math;

			const auto roll	= 0.0;
			const auto pitch= deg2rad(elevation);
			const auto yaw	= deg2rad(azimuth);

			math::Quaternion q =
				AngleAxis(yaw, Vector3::UnitZ())
				* AngleAxis(pitch, Vector3::UnitY())
				* AngleAxis(roll, Vector3::UnitX());

			return q;
		}

		Float length;
		Float azimuth;
		Float elevation;
	};
}

#endif
