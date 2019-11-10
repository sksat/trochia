#ifndef COORDINATE_BODY_HPP_
#define COORDINATE_BODY_HPP_

namespace coordinate::body {
	class Body {
	public:
		Body() : vec(0.0, 0.0, 0.0) {}
		Body(const Float &x, const Float &y, const Float &z) : vec(x,y,z) {}

		Vector3 vec;

		template<typename LocalFrame>
		inline auto to_local(const Quaternion &q) const -> const LocalFrame {
			local::NED ned;
			ned.vec = dcm::body2ned(q) * this->vec;

			return ned;
		}
	};
}

#endif
