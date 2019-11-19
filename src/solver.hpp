#ifndef SOLVER_HPP_
#define SOLVER_HPP_

#include <functional>
#include "math.hpp"

namespace solver {
	using math::Float;

	// 変数xと，導関数dx/dt = f(t, x)を使って数値積分するソルバ群

	// f(t, x)の関数の型
	template<typename T>
	using func_t = auto (*)(const Float &t, const T &x) -> const T;

	// ソルバのベースクラス
	template<typename T>
	class solver {
	public:
		solver(T &x, func_t<T> f) : t(0.0), x(x), f(f) {}

		math::Float t;
		std::reference_wrapper<T> x;
		func_t<T> f;

		// ステップ実行
		virtual auto step(const Float &dt) -> void = 0;
	};

	// オイラー法
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
}

#endif
