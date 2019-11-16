#ifndef SOLVER_HPP_
#define SOLVER_HPP_

#include <functional>
#include "math.hpp"

namespace solver {
	using math::Float;

	template<typename T>
	class func_t {
	public:
		using type = auto (*)(const Float &t, const T &x) -> T;

		func_t(const type fn) : fn(fn) {}

		type fn;

		auto operator()(const Float &t, const T &x) const -> T {
			return this->fn(t, x);
		}
	};

	template<typename T>
	class solver {
	public:
		solver(T &x, func_t<T> f) : x(x), f(f) {}

		std::reference_wrapper<T> x;
		func_t<T> f;

		virtual auto step(const Float &t, const Float &dt) -> void = 0;
	};

	template<typename T>
	class euler : public solver<T> {
	public:
		euler(T &x, func_t<T> f) : solver<T>(x, f) {}

		auto step(const Float &t, const Float &dt) -> void {
			const auto &x = this->x.get();
			const auto &f = this->f;
			this->x.get() = x + dt*f(t, x);
		}
	};

	template<typename T>
	class RK4 : public solver<T> {
	public:
		RK4(T &x, func_t<T> f) : solver<T>(x, f) {}

		auto step(const Float &t, const Float &dt) -> void {
			const auto &x = this->x.get();
			const auto &f = this->f;
			const auto dt_2 = 0.5*dt;
			const auto k1 = f(t,			x);
			const auto k2 = f(t + dt_2,		x + k1*dt_2);
			const auto k3 = f(t + dt_2,		x + k2*dt_2);
			const auto k4 = f(t + dt,		x + k3*dt);
			this->x.get() = x + dt_2 * (k1 + 2*k2 + 2*k3 + k4);
		}
	};
}

#endif
