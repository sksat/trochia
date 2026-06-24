// A freshly constructed Object must have a well-defined initial state.
//
// Regression test for issue #37: Object left time/quat/omega/domega
// uninitialised (Eigen does not zero-initialise), and exec() never set omega,
// so do_step read garbage angular velocity -> undefined, platform/run-dependent
// attitude. We placement-new an Object over a buffer poisoned with 0xFF so any
// member the constructor fails to initialise is deterministically non-zero.
#include <gtest/gtest.h>
#include <new>
#include <cstring>
#include "coordinate.hpp"
#include "object.hpp"

using Obj = trochia::object::Object<trochia::coordinate::local::NED>;

TEST(ObjectInit, DefaultStateIsWellDefined) {
	alignas(Obj) unsigned char buf[sizeof(Obj)];
	std::memset(buf, 0xFF, sizeof(buf));

	Obj *o = new (buf) Obj();

	// angular velocity / its derivative must start at rest
	EXPECT_DOUBLE_EQ(o->omega.x(), 0.0);
	EXPECT_DOUBLE_EQ(o->omega.y(), 0.0);
	EXPECT_DOUBLE_EQ(o->omega.z(), 0.0);
	EXPECT_DOUBLE_EQ(o->domega.x(), 0.0);
	EXPECT_DOUBLE_EQ(o->domega.y(), 0.0);
	EXPECT_DOUBLE_EQ(o->domega.z(), 0.0);

	// attitude must start as the identity rotation
	EXPECT_DOUBLE_EQ(o->quat.w(), 1.0);
	EXPECT_DOUBLE_EQ(o->quat.x(), 0.0);
	EXPECT_DOUBLE_EQ(o->quat.y(), 0.0);
	EXPECT_DOUBLE_EQ(o->quat.z(), 0.0);

	// time must start at zero
	EXPECT_DOUBLE_EQ(o->time, 0.0);

	o->~Obj();
}
