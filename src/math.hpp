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

#ifndef MATH_HPP_
#define MATH_HPP_

#include <cmath>
#include <Eigen/Geometry>

namespace trochia::math {
	// type
	using Float = double;
	
	const Float nan = std::nan("");

	using Vector3	= Eigen::Matrix<Float, 3, 1>;
	using Vector4	= Eigen::Matrix<Float, 4, 1>;
	using Matrix3	= Eigen::Matrix<Float, 3, 3>;
	using Matrix4	= Eigen::Matrix<Float, 4, 4>;
	using Quaternion= Eigen::Quaternion<Float>;

	using AngleAxis	= Eigen::AngleAxis<Float>;

	// constant
	constexpr Float pi = 3.141592653589793238462643383279;

	// func
	template<typename T>
	constexpr auto abs(const T v) -> const T {
		if(v < T(0.0)) return -v;
		return v;
	}

	template<typename T>
	constexpr auto sqrt(const T &s) -> const T {
		// babylonian method

		T x = s / T(2.0);
		T last = T(0.0);

		while(x != last){
			last = x;
			x = (x + s/x) / T(2.0);
		}
		return x;
	}

	template<typename T>
	constexpr auto sin(T x) -> const T {
		T x_sq = -(x * x);
		T series = x;
		T tmp = x;
		T fact= T(2.0);

		do {
			tmp *= x_sq / (fact * (fact + T(1.0)));
			series += tmp;
			fact += T(2.0);
		} while(abs(tmp) >= std::numeric_limits<T>::epsilon());

		return series;
	}

	constexpr auto deg2rad(const Float &deg) -> Float {
		return deg * (pi / 180.0);
	}
	constexpr auto rad2deg(const Float &rad) -> Float {
		return rad * (180.0 / pi);
	}

	inline auto vec2quat(const Vector4 &v) -> Quaternion {
		Quaternion q;
		q.w() = v(0);
		q.x() = v(1);
		q.y() = v(2);
		q.z() = v(3);
		return q;
	}

	inline auto quat2vec(const Quaternion &q) -> Vector4 {
		Vector4 v;
		v(0) = q.w();
		v(1) = q.x();
		v(2) = q.y();
		v(3) = q.z();
		return v;
	}

	[[deprecated("please use AngleAxis")]]
	inline auto euler2quat(const Float &roll, const Float &pitch, const Float &yaw) -> Quaternion {
		using Eigen::AngleAxisd;

		Quaternion q;
		q = AngleAxis(roll, Vector3::UnitX())
			* AngleAxis(pitch, Vector3::UnitY())
			* AngleAxis(yaw, Vector3::UnitZ());
		return q;
	}

	inline auto quat2euler(const Quaternion &q) -> Vector3 {
		const auto m = q.matrix();
		Vector3 e;
		e.x() = std::atan2(m(2,1), m(2,2));
		e.y() = std::atan2(-1.0*m(2,0), std::pow(m(2,1)*m(2,1)+m(2,2)*m(2,2), 0.5));
		e.z() = std::atan2(m(1,0),m(0,0));
		return e;
	}

	inline auto lerp(const Float &a, const Float &b, const Float &t) -> Float {
		return a + t*(b - a);
	}
}

#endif
