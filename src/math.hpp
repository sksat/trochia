#ifndef MATH_HPP_
#define MATH_HPP_

#include <cmath>
#include <Eigen/Geometry>

namespace math {
	using Float = double;
	
	const Float nan = std::nan("");

	using Vector3	= Eigen::Matrix<Float, 3, 1>;
	using Quaternion= Eigen::Quaternion<Float>;

	auto lerp(const Float &a, const Float &b, const Float &t) -> const Float {
		return a + t*(b - a);
	}
}

#endif
