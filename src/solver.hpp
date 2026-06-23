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

#ifndef SOLVER_HPP_
#define SOLVER_HPP_

#include <functional>
#include "math.hpp"

namespace trochia::solver {
	using math::Float;

	// numerical integration solvers
	// use x & dx/dt = f(t, x)

	// type of f(t, x)
	template<typename T>
	using func_t = auto (*)(const Float &t, const T &x) -> const T;

	// base class of solver
	template<typename T>
	class solver {
	public:
		solver(T &x, func_t<T> f) : t(0.0), x(x), f(f) {}

		math::Float t;
		std::reference_wrapper<T> x;
		func_t<T> f;

		virtual auto step(const Float &dt) -> void = 0;
	};

	// Euler method
	template<typename T>
	class euler : public solver<T> {
	public:
		euler(T &x, func_t<T> f) : solver<T>(x,f) {}

		auto step(const Float &dt) -> void {
			const auto &t = this->t;
			const auto &x = this->x.get();
			const auto &f = this->f;

			this->x.get() += f(t, x) * dt;
			this->t += dt;
		}
	};

	// RK2
	template<typename T>
	class RK2 : public solver<T> {
	public:
		RK2(T &x, func_t<T> f) : solver<T>(x,f) {}

		auto step(const Float &dt) -> void {
			const auto &t = this->t;
			const auto &x = this->x.get();
			const auto &f = this->f;

			const auto dt2 = dt / Float(2.0);

			const auto k  =  f(t,     x);
			this->x.get() += f(t+dt2, x + k*dt2) * dt;
			this->t += dt;
		}
	};

	// RK4
	template<typename T>
	class RK4 : public solver<T> {
	public:
		RK4(T &x, func_t<T> f) : solver<T>(x,f) {}

		auto step(const Float &dt) -> void {
			const auto &t = this->t;
			const auto &x = this->x.get();
			const auto &f = this->f;

			const auto dt2 = dt / Float(2.0);

			const auto k1 = f(t,     x);
			const auto k2 = f(t+dt2, x + k1*dt2);
			const auto k3 = f(t+dt2, x + k2*dt2);
			const auto k4 = f(t+dt,  x + k3*dt);
			const auto tmp= (k1 + k2*Float(2.0) + k3*Float(2.0) + k4) / Float(6.0);
			this->x.get() += tmp * dt;
			this->t += dt;
		}
	};
}

#endif
