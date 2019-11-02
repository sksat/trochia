#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include "math.hpp"

// *data.begin()は(0.0, thrust)
// *data.end()は(time_max, 0.0)となるようにすること

class Engine {
public:
	Engine(){}

	using thrust_t = std::pair<math::Float, math::Float>;
	using thrustcurve_t = std::vector<thrust_t>;

	// 推力履歴を読み込む(そのうちRESMに切り替える)
	auto load_data(const std::string &fname) -> void {
		std::ifstream ifs(fname);
		if(!ifs) return;

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
	thrustcurve_t data;
	thrustcurve_t::const_iterator itr;
	math::Float time_max;
};

#endif
