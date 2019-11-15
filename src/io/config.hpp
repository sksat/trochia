#ifndef IO_CONFIG_HPP_
#define IO_CONFIG_HPP_

#include <string>
#include <vector>

class Simulation;

namespace io::config {
	auto load(const std::string &fname, std::vector<Simulation> &sims) -> void;
}

#endif
