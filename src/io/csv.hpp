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

#ifndef IO_CSV_HPP_
#define IO_CSV_HPP_

#include <string>
#include <map>
#include <array>
#include <vector>
#include <fstream>

namespace trochia::io {
	template<typename T, size_t N=2>
	class CSV {
	public:
		using header_t	= std::array<std::string, N>;
		using line_t	= std::array<T, N>;

		CSV(const std::fstream &file) : file(file) {}
		CSV(const std::fstream &file, const header_t &header) : file(file), header(header) {}

		auto operator<<(const line_t &line) -> void {
			for(const auto &e : line){
				file << std::to_string(e);
				if(e != line.cend())
					file << ", ";
			}
			file << std::endl;
		}
	private:
		std::fstream file;
		header_t header;
	};
}

#endif
