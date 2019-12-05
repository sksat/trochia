#ifndef ENVIRONMENT_LAUNCHER_HPP_
#define ENVIRONMENT_LAUNCHER_HPP_

#include "../math.hpp"

namespace trochia::environment {
	using math::Float;

	class Launcher {
	public:
		explicit Launcher(const Float &l) : length(l) {}

	protected:
		Float length;
		math::Quaternion angle;

	public:
		auto get_angle() const -> const math::Quaternion { return this->angle; }

		auto set_angle(const math::Vector3 &euler) -> void {
			this->set_angle(euler.x(), euler.y(), euler.z());
		}
		auto set_angle(const Float &roll, const Float &pitch, const Float yaw) -> void {
			using math::Vector3, math::AngleAxis;

			angle =
				AngleAxis(yaw, Vector3::UnitZ())
				* AngleAxis(pitch, Vector3::UnitY())
				* AngleAxis(roll, Vector3::UnitX());
		}

		// set azimuth by deg
		auto azimuth(const Float &azimuth) -> void {
			const auto yaw = math::deg2rad(azimuth);
			auto euler = math::quat2euler(this->angle);
			euler.z() = yaw;
			this->set_angle(euler);
		}

		// set elevation by deg
		auto elevation(const Float &elevation) -> void {
			const auto pitch = math::deg2rad(elevation);
			auto euler = math::quat2euler(this->angle);
			euler.y() = pitch;
			this->set_angle(euler);
		}
	};
}

#endif
