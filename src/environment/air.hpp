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

#ifndef ENVIRONMENT_AIR_HPP_
#define ENVIRONMENT_AIR_HPP_

#include <array>
#include <utility>
#include "../math.hpp"
#include "temperature.hpp"

// U.S. Standard Atmosphere, 1976 (USSA1976): https://ntrs.nasa.gov/citations/19770009539
//
// All inputs are *geopotential* height H [m] (convert geometric Z with
// earth::geodesy::potential_height first). The 7 base layers reach H = 84852 m
// (~86 km geometric); above that we extrapolate isothermally so density keeps
// falling smoothly toward zero (never NaN), which is enough for a high-altitude
// ballistic / hazard-zone estimate even though the real thermosphere is richer.
//
// At and below 11 km this matches the previous troposphere model to ~1e-5
// (same lapse and P0; the old rho = 0.0034837*P/T is P/(R*T) with R ~= 287.05,
// differing from R = 287.0528 only at the 5th digit), so existing low-altitude
// results are unchanged to the precision anyone reads them at.
namespace trochia::environment::air {
	namespace ussa1976 {
		constexpr math::Float g0 = 9.80665;        // standard gravity [m/s^2]
		constexpr math::Float R  = 287.0528;       // specific gas const = R*/M0 = 8.31432/0.0289644
		constexpr math::Float P0 = 101325.0;       // sea-level pressure [Pa]
		constexpr math::Float T0 = 288.15;         // sea-level temperature [K]
		constexpr math::Float H_top = 84852.0;     // top of the tabulated layers [m]

		// {base geopotential height [m], lapse rate [K/km]} for layers 0..6.
		// Base temperatures follow by continuity from T0 (216.65, 216.65, 228.65,
		// 270.65, 270.65, 214.65), and T at H_top works out to 186.946 K.
		inline auto layers() -> const std::array<std::pair<math::Float, math::Float>, 7> & {
			static const std::array<std::pair<math::Float, math::Float>, 7> table = {{
				{     0.0, -6.5 },
				{ 11000.0,  0.0 },
				{ 20000.0,  1.0 },
				{ 32000.0,  2.8 },
				{ 47000.0,  0.0 },
				{ 51000.0, -2.8 },
				{ 71000.0, -2.0 },
			}};
			return table;
		}

		// temperature [K] and pressure [Pa] at geopotential height H [m].
		inline auto props(math::Float H) -> std::pair<math::Float, math::Float> {
			if(H < 0.0) H = 0.0;	// clamp below sea level
			const auto &layer = layers();
			math::Float Hb = layer[0].first, Tb = T0, Pb = P0;
			for(std::size_t i = 0; i < layer.size(); ++i){
				const math::Float L  = layer[i].second / 1000.0;	// K/km -> K/m
				const math::Float Ht = (i + 1 < layer.size()) ? layer[i + 1].first : H_top;
				if(H <= Ht){
					const math::Float T = Tb + L * (H - Hb);
					const math::Float P = (L == 0.0)
						? Pb * std::exp(-g0 * (H - Hb) / (R * Tb))
						: Pb * std::pow(T / Tb, -g0 / (R * L));
					return { T, P };
				}
				// advance the running base (T, P) to the top of this layer
				const math::Float Tt = Tb + L * (Ht - Hb);
				Pb = (L == 0.0)
					? Pb * std::exp(-g0 * (Ht - Hb) / (R * Tb))
					: Pb * std::pow(Tt / Tb, -g0 / (R * L));
				Tb = Tt;
				Hb = Ht;
			}
			// above H_top: isothermal extrapolation (Tb = 186.946 K, Pb = P(H_top))
			const math::Float P = Pb * std::exp(-g0 * (H - Hb) / (R * Tb));
			return { Tb, P };
		}
	}

	// temperature [K] at geopotential height H [m]
	inline auto temperature(const math::Float &H) -> temperature::kelvin {
		return temperature::kelvin(ussa1976::props(H).first);
	}

	// pressure [Pa] at geopotential height H [m]
	inline auto pressure(const math::Float &H) -> math::Float {
		return ussa1976::props(H).second;
	}

	// density [kg/m^3] at geopotential height H [m]
	inline auto density(const math::Float &H) -> math::Float {
		const auto [T, P] = ussa1976::props(H);
		return P / (ussa1976::R * T);
	}
}

#endif
