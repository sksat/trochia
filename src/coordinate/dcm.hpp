#ifndef COORDINATE_DCM_HPP_
#define COORDINATE_DCM_HPP_

namespace coordinate::dcm {
	inline auto ned2body(const Quaternion &q_) -> const Matrix3 {
		Matrix3 mat;
		const auto &q = q_.coeffs();
		const auto q00 = q(0) * q(0);
		const auto q11 = q(1) * q(1);
		const auto q22 = q(2) * q(2);
		const auto q33 = q(3) * q(3);
		mat <<
			q00 - q11 - q22 + q33,		2*(q(0)*q(1) + q(2)*q(3)),	2*(q(0)*q(2) - q(1)*q(3)),
			2*(q(0)*q(1) - q(2)*q(3)),	q11 - q00 - q22 + q33,		2*(q(1)*q(2) + q(0)*q(3)),
			2*(q(0)*q(2) + q(1)*q(3)),	2*(q(1)*q(2) - q(0)*q(3)),	q22 - q00 - q11 + q33;
		return mat;
	}
	inline auto body2ned(const Quaternion &q) -> const Matrix3 {
		return ned2body(q).transpose();
	}
}

#endif
