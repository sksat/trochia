#ifndef ROCKET_HPP_
#define ROCKET_HPP_

#include <memory>
#include <string>
#include "math.hpp"
#include "coordinate.hpp"
#include "engine.hpp"

#include "object.hpp"

namespace rocket {
	using LocalFrame = coordinate::local::NED;

	class Rocket : public object::Object<LocalFrame> {
	public:
		std::string name;
		Engine engine;

		auto weight() const -> const math::Float {
			return 10.0 + engine.weight(this->time);
		}
	};
}

#endif
