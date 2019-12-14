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
		auto get_angle() const -> math::Quaternion { return this->angle; }

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
