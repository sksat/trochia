#ifndef ENVIRONMENT_GRAVITY_HPP_
#define ENVIRONMENT_GRAVITY_HPP_

#include "../math.hpp"

namespace environment {
	namespace constant {
		// 2018 CODATA recommended values
		constexpr math::Float G		= 6.6743015e-11;

		// standard acceleration of gravity(definition)
		constexpr math::Float gn	= 9.80665;

		// Chronological Scientific Tables 2014
		constexpr math::Float re	= 6.3781366e6;
	}

	// TODO
	// latitude model(Helmert equation, WGS-84)
	// gravitation model
	// other planet

	template<typename T>
	inline auto gravity(const T &pos, T &acc) -> void {
		using namespace constant;

		const auto tmp = re / (re + pos.altitude());
		const auto g = gn * tmp * tmp;

		acc.altitude(acc.altitude() + -1.0 * g);
	}
}

#endif
