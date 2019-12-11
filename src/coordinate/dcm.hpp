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

#ifndef COORDINATE_DCM_HPP_
#define COORDINATE_DCM_HPP_

namespace trochia::coordinate::dcm {
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
