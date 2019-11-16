#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "math.hpp"

namespace object {
	template<typename Frame>
	class Object {
	public:
		Object() {}
		Object(const Frame pos) : pos(pos) {}

		math::Float time;
		Frame pos, vel, acc;
		math::Quaternion quat;

		virtual auto weight() const -> const math::Float = 0;

		inline auto force(const Frame &f) -> void {
			this->acc = f;
			this->acc /= this->weight();
		}
	};
}

#endif
