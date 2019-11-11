#ifndef COORDINATE_EARTH_HPP_
#define COORDINATE_EARTH_HPP_

namespace coordinate::earth {
	namespace ellipsoid {
		class ellipsoid {
		public:
			constexpr ellipsoid(Float a, Float f_inv) : a(a), f_inv(f_inv),
				f(0.0), b(0.0), e(0.0), e2(0.0)
			{
				calc_parameters();
			}

			constexpr auto calc_parameters() -> void {
				// https://psgsv2.gsi.go.jp/koukyou/jyunsoku/pdf/h28/h28_junsoku_furoku6.pdf
				f = 1.0 / f_inv;
				b = a * (f_inv - 1.0) / f_inv;

				const auto f_tmp = 2.0 * f_inv - 1.0;
				e = std::sqrt(f_tmp) / f_inv;
				e2= f_tmp / (f_inv * f_inv);
			}

			constexpr auto W(const Float &lat) const -> const Float {
				const auto sin2	= std::sin(lat) * std::sin(lat);
				return std::sqrt(1.0 - e2*sin2);
			}

			constexpr auto N(const Float &lat) const -> const Float {
				return a / W(lat);
			}

			constexpr auto M(const Float &lat) const -> const Float {
				const auto w = W(lat);
				const auto W2= w*w;
				return N(lat) * (1.0 - e2) / W2;
			}

			Float a, f_inv;
			Float f, b, e, e2;
		};

		// https://www.jmu.edu/cisr/research/sic/standards/ellipsoid.htm
		constexpr auto Airy_1830			= ellipsoid(6'377'563,		299.33);
		constexpr auto Everest_1830			= ellipsoid(6'377'276.3,	300.80);
		constexpr auto Bessel_1841			= ellipsoid(6'377'397.2,	299.15);
		constexpr auto Clarke_1866			= ellipsoid(6'378'206.4,	294.98);
		constexpr auto Clarke_1880			= ellipsoid(6'378'249.2,	293.47);
		constexpr auto International_1924	= ellipsoid(6'378'388,		297);
		constexpr auto Krasovsky_1924		= ellipsoid(6'378'245,		298.3);
		constexpr auto International_Astronomical_Union_1968
											= ellipsoid(6'378'160,		298.25);
		constexpr auto WGS72				= ellipsoid(6'378'135,		298.26);
		constexpr auto GRS80				= ellipsoid(6'378'137,		298.26);
		constexpr auto WGS84				= ellipsoid(GRS80.a,		298.25722);
	}
}

#endif
