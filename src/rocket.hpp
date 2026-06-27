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

#ifndef ROCKET_HPP_
#define ROCKET_HPP_

#include <memory>
#include <optional>
#include <string>
#include "math.hpp"
#include "coordinate.hpp"
#include "engine.hpp"

#include "object.hpp"

namespace trochia::rocket {
	using LocalFrame = coordinate::local::NED;

	class Rocket : public object::Object<LocalFrame> {
	public:
		Rocket(){}
		Rocket(const object::Object<LocalFrame> &o) : object::Object<LocalFrame>(o) {}

		std::string name;
		Engine engine;

		math::Float diameter;
		math::Float length;
		math::Float mass;				// without engine
		math::Float lcg0, lcgf, lcgp;	// Center of Gravity
		math::Float lcp;				// Center of Pressure
		math::Float I0, If;				// Moment of inertia
		math::Float Cd;					// Coefficient of Drag
		math::Float Cna;

		math::Float Cmq;

		math::Float angle_attack, angle_side_slip;
		math::Float D, N, Y;

		math::Float va_max=0.0, N_max=0.0;

		// parachute / recovery. para_cd<=0 or para_area<=0 means no parachute
		// (the rocket descends ballistically). The chute deploys at apogee plus
		// para_delay; once open the body aerodynamics/rotation are dropped and
		// the rocket descends under chute drag, drifting with the wind.
		math::Float para_cd    = 0.0;	// parachute drag coefficient
		math::Float para_area  = 0.0;	// parachute reference area [m^2]
		math::Float para_delay = 0.0;	// deploy delay after apogee [s]
		bool chute_open = false;	// runtime: is the chute deployed?
		std::optional<math::Float> apogee_time;	// runtime: set when the climb turns to descent

		static auto dx(const math::Float &t, const Rocket &r) -> const Rocket {
			return object::Object<LocalFrame>::dx(t, r);
		}

		auto lcg() const -> math::Float {
			return math::lerp(lcg0, lcgf, engine.progress(this->time));
		}

		auto inertia() const -> math::Float {
			return math::lerp(I0, If, engine.progress(this->time));
		}

		auto weight() const -> math::Float {
			return mass + engine.weight(this->time);
		}
	};
}

#endif
