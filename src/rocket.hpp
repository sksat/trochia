#ifndef ROCKET_HPP_
#define ROCKET_HPP_

#include <memory>
#include <string>
#include "math.hpp"
#include "coordinate.hpp"
#include "engine.hpp"

#include "object.hpp"

namespace trochia::rocket {
	using LocalFrame = coordinate::local::NED;

	class Rocket : public object::Object<LocalFrame> {
	public:
		Rocket(){}
		Rocket(const object::Object<LocalFrame> &o) : object::Object<LocalFrame>(o) {}

		std::string name;
		Engine engine;

		math::Float lcg0, lcgf;		// 重心位置

		static auto dx(const math::Float &t, const Rocket &r) -> const Rocket {
			return object::Object<LocalFrame>::dx(t, r);
		}

		auto lcg() const -> const math::Float {
			return math::lerp(lcg0, lcgf, engine.progress(this->time));
		}

		auto weight() const -> const math::Float {
			return 10.0 + engine.weight(this->time);
		}
	};
}

#endif
