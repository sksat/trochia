#ifndef MATH_HPP_
#define MATH_HPP_

#include <cmath>
#include <Eigen/Geometry>

namespace trochia::math {
	// type
	using Float = double;
	
	const Float nan = std::nan("");

	using Vector3	= Eigen::Matrix<Float, 3, 1>;
	using Matrix3	= Eigen::Matrix<Float, 3, 3>;
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

	constexpr auto deg2rad(const Float &deg) -> const Float {
		return deg * (pi / 180.0);
	}
	constexpr auto rad2deg(const Float &rad) -> const Float {
		return rad * (180.0 / pi);
	}

	[[deprecated("please use AngleAxis")]]
	inline auto euler2quat(const Float &roll, const Float &pitch, const Float &yaw) -> const Quaternion {
		using Eigen::AngleAxisd;

		Quaternion q;
		q = AngleAxisd(roll, Vector3::UnitX())
			* AngleAxisd(pitch, Vector3::UnitY())
			* AngleAxisd(yaw, Vector3::UnitZ());
		return q;
	}

	inline auto quat2euler(const Quaternion &q) -> const Vector3 {
		return q.toRotationMatrix().eulerAngles(0, 1, 2);
	}

	inline auto lerp(const Float &a, const Float &b, const Float &t) -> const Float {
		return a + t*(b - a);
	}
}

#endif
