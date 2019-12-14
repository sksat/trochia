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

#ifndef ENVIRONMENT_EARTH_ELLIPSOID_HPP_
#define ENVIRONMENT_EARTH_ELLIPSOID_HPP_

#include "../../math.hpp"

namespace trochia::environment::earth {
	namespace ellipsoid {
		using math::Float;

		class Ellipsoid {
		public:
			constexpr Ellipsoid(Float a, Float f_inv) : a(a), f_inv(f_inv),
				f(0.0), b(0.0), e(0.0), e2(0.0)
			{
				calc_parameters();
			}

			constexpr auto calc_parameters() -> void {
				// https://psgsv2.gsi.go.jp/koukyou/jyunsoku/pdf/h28/h28_junsoku_furoku6.pdf
				f = 1.0 / f_inv;
				b = a * (f_inv - 1.0) / f_inv;

				const auto f_tmp = 2.0 * f_inv - 1.0;
				e = math::sqrt(f_tmp) / f_inv;
				e2= f_tmp / (f_inv * f_inv);
			}

			constexpr auto W(const Float &lat) const -> Float {
				const auto sin  = math::sin(lat);
				const auto sin2	= sin*sin;
				return math::sqrt(1.0 - e2*sin2);
			}

			constexpr auto N(const Float &lat) const -> Float {
				return a / W(lat);
			}

			constexpr auto M(const Float &lat) const -> Float {
				const auto w = W(lat);
				const auto W2= w*w;
				return N(lat) * (1.0 - e2) / W2;
			}

			Float a, f_inv;
			Float f, b, e, e2;
		};

		// https://www.jmu.edu/cisr/research/sic/standards/ellipsoid.htm
		constexpr auto Airy_1830			= Ellipsoid(6'377'563,		299.33);
		constexpr auto Everest_1830			= Ellipsoid(6'377'276.3,	300.80);
		constexpr auto Bessel_1841			= Ellipsoid(6'377'397.2,	299.15);
		constexpr auto Clarke_1866			= Ellipsoid(6'378'206.4,	294.98);
		constexpr auto Clarke_1880			= Ellipsoid(6'378'249.2,	293.47);
		constexpr auto International_1924	= Ellipsoid(6'378'388,		297);
		constexpr auto Krasovsky_1924		= Ellipsoid(6'378'245,		298.3);
		constexpr auto International_Astronomical_Union_1968
											= Ellipsoid(6'378'160,		298.25);
		constexpr auto WGS72				= Ellipsoid(6'378'135,		298.26);
		constexpr auto GRS80				= Ellipsoid(6'378'137,		298.26);
		constexpr auto WGS84				= Ellipsoid(GRS80.a,		298.25722);
	}
}

#endif
