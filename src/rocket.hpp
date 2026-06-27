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
#include <vector>
#include "math.hpp"
#include "coordinate.hpp"
#include "engine.hpp"

#include "object.hpp"

namespace trochia::rocket {
	using LocalFrame = coordinate::local::NED;

	// One recovery stage. Any number of these can be chained (drogue, main, ...).
	// A stage deploys at apogee+delay (at_apogee) or when descending through
	// `altitude` (AGL). While any stage is open the rocket descends under the
	// summed chute drag.
	struct Parachute {
		bool at_apogee = false;             // deploy at apogee + delay
		math::Float delay = 0.0;            // [s] after apogee
		std::optional<math::Float> altitude;// else deploy at this altitude AGL [m] on descent
		math::Float cd = 0.0;               // drag coefficient
		math::Float area = 0.0;             // reference area [m^2]
		bool open = false;                  // runtime: deployed?
	};

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

		// recovery: any number of parachute stages (empty -> ballistic descent).
		// While any stage is open the body aero/rotation are dropped and the
		// rocket descends under the summed chute drag, drifting with the wind.
		std::vector<Parachute> parachutes;
		std::optional<math::Float> apogee_time;	// runtime: set when the climb turns to descent

		// is any recovery stage deployed?
		auto chute_open() const -> bool {
			for(const auto &p : parachutes) if(p.open) return true;
			return false;
		}
		// summed drag area (Cd*A) of the open stages
		auto chute_cda() const -> math::Float {
			math::Float s = 0.0;
			for(const auto &p : parachutes) if(p.open) s += p.cd * p.area;
			return s;
		}

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
