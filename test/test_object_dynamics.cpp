// Quaternion kinematics in Object::dx.
//
// Regression test for issue #37: dq/dt must be 0.5 * (q (x) omega_quat) for
// body angular rates (Hamilton product). quat2vec/vec2quat use the scalar-first
// ordering [w,x,y,z], but dx used the scalar-last Omega matrix, so the attitude
// derivative was wrong (a "[fix] quaternion" earlier only added the 0.5 factor,
// not the ordering).
#include <gtest/gtest.h>
#include "coordinate.hpp"
#include "object.hpp"

using trochia::math::Quaternion;
using trochia::math::Vector3;
using Obj = trochia::object::Object<trochia::coordinate::local::NED>;

namespace { constexpr double tol = 1e-12; }

// For the identity quaternion, dq vector part = 0.5*omega, scalar part 0.
TEST(ObjectDx, IdentityQuatRateX) {
	Obj o; o.quat = Quaternion(1, 0, 0, 0); o.omega = Vector3(2.0, 0, 0);
	const auto d = Obj::dx(0.0, o);
	EXPECT_NEAR(d.quat.w(), 0.0, tol);
	EXPECT_NEAR(d.quat.x(), 1.0, tol);
	EXPECT_NEAR(d.quat.y(), 0.0, tol);
	EXPECT_NEAR(d.quat.z(), 0.0, tol);
}

TEST(ObjectDx, IdentityQuatRateZ) {
	Obj o; o.quat = Quaternion(1, 0, 0, 0); o.omega = Vector3(0, 0, 2.0);
	const auto d = Obj::dx(0.0, o);
	EXPECT_NEAR(d.quat.w(), 0.0, tol);
	EXPECT_NEAR(d.quat.x(), 0.0, tol);
	EXPECT_NEAR(d.quat.y(), 0.0, tol);
	EXPECT_NEAR(d.quat.z(), 1.0, tol);
}

// General case: dq must equal 0.5 * (q (x) omega_quat), computed via Eigen.
TEST(ObjectDx, MatchesHamiltonProduct) {
	Obj o;
	const Quaternion q(0.5, 0.5, 0.5, 0.5); // unit quaternion
	const Vector3 w(0.3, -0.7, 1.1);
	o.quat = q; o.omega = w;

	const auto d = Obj::dx(0.0, o);

	const Quaternion wq(0.0, w.x(), w.y(), w.z());
	const Quaternion prod = q * wq; // Hamilton product

	EXPECT_NEAR(d.quat.w(), 0.5 * prod.w(), tol);
	EXPECT_NEAR(d.quat.x(), 0.5 * prod.x(), tol);
	EXPECT_NEAR(d.quat.y(), 0.5 * prod.y(), tol);
	EXPECT_NEAR(d.quat.z(), 0.5 * prod.z(), tol);
}

// Rotation preserves the quaternion norm: q . q_dot = 0.
TEST(ObjectDx, NormPreserved) {
	Obj o;
	const Quaternion q(0.5, 0.5, 0.5, 0.5);
	o.quat = q; o.omega = Vector3(0.3, -0.7, 1.1);
	const auto d = Obj::dx(0.0, o);
	const double dot = q.w()*d.quat.w() + q.x()*d.quat.x()
		+ q.y()*d.quat.y() + q.z()*d.quat.z();
	EXPECT_NEAR(dot, 0.0, tol);
}
