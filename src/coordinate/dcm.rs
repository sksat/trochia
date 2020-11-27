extern crate nalgebra as na;
use na::{ArrayStorage, Matrix, Quaternion, U3};

use crate::types::Float;

type DCM = Matrix<Float, U3, U3, ArrayStorage<Float, U3, U3>>;

#[rustfmt::skip]
pub fn ned2body(q: Quaternion<Float>) -> DCM {
    let q00 = q[0] * q[0];
    let q11 = q[1] * q[1];
    let q22 = q[2] * q[2];
    let q33 = q[3] * q[3];

    DCM::new(
        q00 - q11 - q22 + q33,               2.0 * (q[0] * q[1] + q[2] * q[3]),   2.0 * (q[0] * q[2] - q[1] * q[3]),
        2.0 * (q[0] * q[1] - q[2] * q[3]),   q11 - q00 - q22 + q33,               2.0 * (q[1] * q[2] + q[0] * q[3]),
        2.0 * (q[0] * q[2] + q[1] * q[3]),   2.0 * (q[1] * q[2] - q[0] * q[3]),   q22 - q00 - q11 + q33,
    )
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::coordinate::{local, Body, Local};

    #[test]
    fn n2b() {
        let n = local::NED::new(0.0, 0.0, 0.0);
        let q = Quaternion::new(0.0, 0.0, 0.0, 0.0);
        let _: Body = (ned2body(q) * n.vec).into();
    }
}
