#ifndef MATH_HPP_
#define MATH_HPP_

#include <cmath>
#include <Eigen/Geometry>

namespace math {
	// type
	using Float = double;
	
	const Float nan = std::nan("");

	using Vector3	= Eigen::Matrix<Float, 3, 1>;
	using Quaternion= Eigen::Quaternion<Float>;

	// constant
	constexpr Float pi = 3.141592653589793238462643383279;

	// func
	constexpr auto deg2rad(const Float &deg) -> const Float {
		return deg * (pi / 180.0);
	}
	constexpr auto rad2deg(const Float &rad) -> const Float {
		return rad * (180.0 / pi);
	}

	auto euler2quat(const Float &roll, const Float &pitch, const Float &yaw) -> const Quaternion {
		using Eigen::AngleAxisd;

		Quaternion q;
		q = AngleAxisd(roll, Vector3::UnitX())
			* AngleAxisd(pitch, Vector3::UnitY())
			* AngleAxisd(yaw, Vector3::UnitZ());
		return q;
	}

	auto quat2euler(const Quaternion &q) -> const Vector3 {
		return q.toRotationMatrix().eulerAngles(0, 1, 2);
	}

	auto lerp(const Float &a, const Float &b, const Float &t) -> const Float {
		return a + t*(b - a);
	}
}

#endif
