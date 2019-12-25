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

#ifndef TROCHIA_VERSION_HPP_
#define TROCHIA_VERSION_HPP_

#include <cstddef>
#include <iostream>

namespace trochia::version {
	constexpr size_t major = 0;
	constexpr size_t minor = 4;

	auto version() -> void {
		using std::endl;
		std::cerr
			<< "Trochia " << major << "." << minor
			<< " (commit: " << GIT_COMMIT_ID << ")" << endl
			<< "commit date: " << GIT_COMMIT_DATE << endl
			<< R"(
Copyright (C) 2019 sksat <sksat@sksat.net>
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions; see LICENSE for details.
	)"
			<< endl;
	}
}

#endif
