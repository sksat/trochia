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

#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include "math.hpp"

// *data.begin()は(0.0, thrust)
// *data.end()は(time_max, 0.0)となるようにすること

namespace trochia {
	class Engine {
	public:
		Engine(){}

		using thrust_t = std::pair<math::Float, math::Float>;
		using thrustcurve_t = std::vector<thrust_t>;

		// engファイルからデータを読み込む
		auto load_eng(const std::string &fname) -> void {
			std::ifstream ifs(fname);
			if(!ifs) return;

			std::string delays;
			ifs >> name >> diameter >> length
				>> delays
				>> weight_prop >> weight_total
				>> manufacturer;

			double t, th;
			thrust_t thrust;
			thrust.first = 0.0;
			thrust.second= 0.0;

			while(ifs){
				ifs >> t >> th;
				if(t <= thrust.first)			// データがソートされていない
					break;

				if(thrust.first == 0.0){
					if(t != 0.0)				// tの初期値が0.0でない
						data.push_back(thrust);
				}

				thrust.first = t;
				thrust.second= th;
				data.push_back(thrust);
			}

			if(th != 0.0){						// 最終推力が0.0でない
				const auto &end = data.cend()-1;
				const auto dt = end->first - (end-1)->first;	// 直前のdtを使う
				thrust.first = end->first + dt;
				thrust.second= 0.0;
				data.push_back(thrust);
			}

			time_max = thrust.first;
			itr = data.cbegin();
		}

		inline auto progress(const math::Float &time) const -> const math::Float {
			return time / time_max;
		}

		inline auto weight(const math::Float &time) const -> const math::Float {
			const auto prop = math::lerp(weight_prop, 0, this->progress(time));
			return weight_total - weight_prop + prop;
		}

		auto thrust(const math::Float &time) -> const math::Float {
			const auto &next = itr+1;
			const auto &time_now	= itr->first;
			const auto &thrust_now	= itr->second;
			const auto &time_next	= next->first;
			const auto &thrust_next	= next->second;

			if(time >= time_max)		// 燃焼終了
				return 0.0;

			// 補間が必要ないやつ
			if(time == time_now)
				return thrust_now;
			if(time == time_next){
				itr++;
				return itr->second;
			}

			// 補間
			if(time_now < time && time < time_next){
				const auto range	= time_next - time_now;
				const auto elepsed	= time - time_now;
				const auto progress	= elepsed / range;
				return math::lerp(thrust_now, thrust_next, progress);
			}

			// 更新
			if(time > time_next){
				if(next != data.cend()-1)
					itr++;
				return thrust(time);
			}

			return 0.0;
		}
	private:
		// info
		std::string name;
		math::Float diameter, length;
		math::Float weight_prop, weight_total;
		std::string manufacturer;

		// thrustcurve
		thrustcurve_t data;
		thrustcurve_t::const_iterator itr;
		math::Float time_max;
	};
}

#endif
