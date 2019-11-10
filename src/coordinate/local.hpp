#ifndef COORDINATE_LOCAL_HPP_
#define COORDINATE_LOCAL_HPP_

namespace coordinate::local {
	class NED {
	public:
		NED() : vec(0.0, 0.0, 0.0) {}

		Vector3 vec;

		auto north(const optFloat n=std::nullopt) -> const Float {
			if(n) vec.x() = n.value();
			return vec.x();
		}

		auto east(const optFloat e=std::nullopt) -> const Float {
			if(e) vec.y() = e.value();
			return vec.y();
		}

		auto down(const optFloat d=std::nullopt) -> const Float {
			if(d) vec.z() = d.value();
			return vec.z();
		}

		auto south(const optFloat s=std::nullopt) -> const Float {
			if(s) north(-1.0 * s.value());
			return -1.0*north();
		}

		auto west(const optFloat w=std::nullopt) -> const Float {
			if(w) east(-1.0*w.value());
			return -1.0*east();
		}

		auto up(const optFloat u=std::nullopt) -> const Float {
			if(u) down(-1.0*u.value());
			return -1.0*down();
		}

		template<typename Frame>
		operator Frame() {
			Frame frame;
			frame.north(this->north());
			frame.east(this->east());
			frame.down(this->down());
			return frame;
		}
	};

	class ENU {
	public:
		ENU() : vec(0.0, 0.0, 0.0) {}

		Vector3 vec;

		auto east(const optFloat e=std::nullopt) -> const Float {
			if(e) vec.x() = e.value();
			return vec.x();
		}

		auto north(const optFloat n=std::nullopt) -> const Float {
			if(n) vec.y() = n.value();
			return vec.y();
		}

		auto up(const optFloat u=std::nullopt) -> const Float {
			if(u) vec.z() = u.value();
			return vec.z();
		}

		auto west(const optFloat w=std::nullopt) -> const Float {
			if(w) east(-1.0 * w.value());
			return -1.0*east();
		}

		auto south(const optFloat s=std::nullopt) -> const Float {
			if(s) north(-1.0*s.value());
			return -1.0*north();
		}

		auto down(const optFloat d=std::nullopt) -> const Float {
			if(d) up(-1.0*d.value());
			return -1.0*up();
		}

		template<typename Frame>
		operator Frame() {
			Frame frame;
			frame.east(this->east());
			frame.north(this->north());
			frame.up(this->up());
			return frame;
		}
	};
}

#endif
