#ifndef COORDINATE_EARTH_HPP_
#define COORDINATE_EARTH_HPP_

#include "../environment/earth.hpp"

namespace trochia::coordinate::earth {
	using namespace environment::earth;
	using ellipsoid::Ellipsoid;

	class LLH {
	public:

		Float lat, lon, height;
	};

	class ECEF {
	public:
		ECEF(Ellipsoid elp=ellipsoid::WGS84) : elp(elp) {}

		Ellipsoid elp;
		Vector3 vec;
	};

	// https://vldb.gsi.go.jp/sokuchi/surveycalc/surveycalc/trans_alg/trans_alg.html
	inline auto llh2ecef(const LLH &llh, ECEF &ecef, const Float n_g=0.0) -> void {
		auto &v = ecef.vec;
		const auto h = llh.height + n_g;
		const auto N = ecef.elp.N(llh.lat);
		const auto tmp_xy = (N + h) * std::cos(llh.lat);

		v.x() = tmp_xy * std::cos(llh.lon);
		v.y() = tmp_xy * std::sin(llh.lon);
		v.z() = (N * (1.0 - ecef.elp.e2)) * std::sin(llh.lat);
	}

	inline auto ecef2llh(const ECEF &ecef, LLH &llh) -> void {
		const auto &elp = ecef.elp;
		const auto &v = ecef.vec;
		llh.lon = std::atan(v.y() / v.x());
		const auto P = std::sqrt(v.x()*v.x() + v.y()*v.y());
		const auto lat_0 = std::atan(v.z() / (P * (1.0 - elp.e2)));
		Float lat = lat_0;
		while(true){
			const auto new_lat = std::atan(v.z() / (P - elp.e2 * elp.N(lat) * std::cos(lat)));
			const auto diff = std::abs(new_lat - lat);
			lat = new_lat;
			if(diff <= 1.0e-12) break;
		}
		llh.lat = lat;
		llh.height = (P / std::cos(lat)) - elp.N(lat);
	}
}

#endif
