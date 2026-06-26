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

#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include "math.hpp"

// *data.begin() = (0.0,     thrust)
// *data.end()   = (time_end,   0.0)

namespace trochia {
	class Engine {
	public:
		Engine(){}

		using thrust_t = std::pair<math::Float, math::Float>;
		using thrustcurve_t = std::vector<thrust_t>;

		// load data from .eng file
		auto load_eng(const std::string &fname) -> void {
			std::ifstream ifs(fname);
			if(!ifs) return;

			// RASP .eng files (e.g. every ThrustCurve.org export) begin with
			// ';' comment lines; strip comment and blank lines before parsing
			// so the header is read from the first real line.
			std::stringstream ss;
			for(std::string line; std::getline(ifs, line); ){
				const auto p = line.find_first_not_of(" \t\r\n");
				if(p == std::string::npos || line[p] == ';')
					continue;
				ss << line << '\n';
			}

			std::string delays;
			ss >> name >> diameter >> length
				>> delays
				>> weight_prop >> weight_total
				>> manufacturer;

			std::cerr << std::endl
				<< "engine data" << std::endl
				<< "\tname: " << name << std::endl
				<< "\tdiameter: " << diameter << std::endl
				<< "\tlength: " << length << std::endl
				<< "\tprop weight: " << weight_prop << std::endl
				<< "\ttotal weight: " << weight_total << std::endl
				<< "\tmanufacturer: " << manufacturer << std::endl;

			double t, th;
			thrust_t thrust;
			thrust.first = 0.0;
			thrust.second= 0.0;

			double thrust_max = 0.0;

			// load thrust curve
			while(ss){
				ss >> t >> th;
				if(t < thrust.first)			// unsorted data
					break;

				if(thrust.first == 0.0){
					if(t != 0.0)				// first t value is not 0.0
						data.push_back(thrust);
				}

				if(th > thrust_max){
					thrust_max = th;
					time_max = t;
				}

				thrust.first = t;
				thrust.second= th;
				data.push_back(thrust);
			}

			if(th != 0.0){						// last thrust is not 0.0
				const auto &end = data.cend()-1;
				const auto dt = end->first - (end-1)->first;	// use latest dt
				thrust.first = end->first + dt;
				thrust.second= 0.0;
				data.push_back(thrust);
			}

			time_end = thrust.first;

			for(auto i=data.crbegin();i!=data.crend();i++){
				if(i->second > this->thrust(time_max)*0.01){
					time_valid = (i-1)->first;
					break;
				}
			}

			std::cerr
				<< "\t" << "max thrust: " << this->thrust(time_max)
					<< "(" << time_max << "s)" << std::endl
				<< "\t" << "valid time: " << time_valid << std::endl;
		}

		inline auto progress(const math::Float &time) const -> math::Float {
			if(time >= time_valid)
				return 1.0;
			return time / time_end;
		}

		inline auto weight(const math::Float &time) const -> math::Float {
			const auto prop = math::lerp(weight_prop, 0, this->progress(time));
			return weight_total - weight_prop + prop;
		}

		// Linear-interpolate the thrust at the given time.
		// Stateless (const) so it does not depend on call order and stays valid
		// when the Engine is copied (the previous version advanced a member
		// iterator and, after a Simulation copy, compared iterators from two
		// different containers -- UB behind issue #37).
		auto thrust(const math::Float &time) const -> math::Float {
			if(data.empty())
				return 0.0;
			if(time <= data.front().first)
				return data.front().second;
			if(time >= time_end)		// thrust curve data is end
				return 0.0;

			// data is sorted by time; find the first point after `time`
			const auto next = std::upper_bound(data.cbegin(), data.cend(), time,
				[](const math::Float t, const thrust_t &p){ return t < p.first; });
			if(next == data.cbegin() || next == data.cend())
				return 0.0;

			const auto &p0 = *(next-1);
			const auto &p1 = *next;
			const auto range = p1.first - p0.first;
			if(range <= 0.0)			// coincident times: avoid divide-by-zero
				return p0.second;

			return math::lerp(p0.second, p1.second, (time - p0.first) / range);
		}
	private:
		// info
		std::string name;
		math::Float diameter, length;
		math::Float weight_prop, weight_total;
		std::string manufacturer;

		// thrustcurve
		thrustcurve_t data;
	public:
		math::Float time_max, time_valid, time_end;
	};
}

#endif
