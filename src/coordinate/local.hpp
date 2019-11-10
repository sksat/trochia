#ifndef COORDINATE_LOCAL_HPP_
#define COORDINATE_LOCAL_HPP_

namespace coordinate::local {
	class NED {
	public:
		NED() : vec(0.0, 0.0, 0.0) {}

		Vector3 vec;

		auto north() const -> const Float { return vec.x(); }
		auto east()  const -> const Float { return vec.y(); }
		auto down()  const -> const Float { return vec.z(); }

		auto south() const -> const Float { return -1.0*north(); }
		auto west()  const -> const Float { return -1.0*east();  }
		auto up()    const -> const Float { return -1.0*down();  }

		auto altitude() const -> const Float { return up(); }

		auto north(const Float &n) -> void{ vec.x() = n; }
		auto east(const Float &e)  -> void{ vec.y() = e; }
		auto down(const Float &d)  -> void{ vec.z() = d; }

		auto south(const Float &s) -> void{ north(-1.0*s); }
		auto west(const Float &w)  -> void{ east(-1.0*w);  }
		auto up(const Float &u)    -> void{ down(-1.0*u);  }

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

		auto east()  const -> const Float { return vec.x(); }
		auto north() const -> const Float { return vec.y(); }
		auto up()    const -> const Float { return vec.z(); }

		auto west()  const -> const Float { return -1.0*east();  }
		auto south() const -> const Float { return -1.0*north(); }
		auto down()  const -> const Float { return -1.0*up();    }

		auto altitude() const -> const Float { return up(); }

		auto east(const Float &e)  -> void{ vec.x() = e; }
		auto north(const Float &n) -> void{ vec.y() = n; }
		auto up(const Float &u)    -> void{ vec.z() = u; }

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
