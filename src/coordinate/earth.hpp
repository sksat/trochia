#ifndef COORDINATE_EARTH_HPP_
#define COORDINATE_EARTH_HPP_

namespace coordinate::earth {
	namespace ellipsoid {
		class ellipsoid {
		public:
			constexpr ellipsoid(Float a, Float f_inv) : a(a), f_inv(f_inv) {}

			Float a, f_inv;
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
