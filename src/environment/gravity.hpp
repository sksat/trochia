#ifndef ENVIRONMENT_GRAVITY_HPP_
#define ENVIRONMENT_GRAVITY_HPP_

#include "../math.hpp"

namespace trochia::environment {
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

	inline auto gravity(const math::Float &altitude) -> const math::Float {
		const auto tmp = constant::re / (constant::re + altitude);
		return constant::gn * tmp * tmp;
	}
}

#endif
