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

#ifndef IO_CONFIG_HPP_
#define IO_CONFIG_HPP_

#include <string>
#include <vector>
#include "../simulation.hpp"

namespace trochia::io::config {
	using cond_elevation= std::vector<math::Float>;
	using cond_wspeed	= std::vector<math::Float>;
	using cond_wdir		= std::vector<math::Float>;
	using Conditions	= std::tuple<cond_elevation, cond_wspeed, cond_wdir>;

	auto load(const std::string &fname, simulation::Simulation &sim) -> Conditions;
}

#endif
