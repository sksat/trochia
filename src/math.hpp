#ifndef MATH_HPP_
#define MATH_HPP_

#include <cmath>
#include <Eigen/Geometry>

namespace math {
	using Float = double;

	using Vector3	= Eigen::Matrix<Float, 3, 1>;
	using Quaternion= Eigen::Quaternion<Float>;
}

#endif
