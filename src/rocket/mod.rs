extern crate dimensioned as dim;

use crate::types::Float;

use dim::si;

pub mod engine;
//pub mod stage;

pub struct Rocket {
    inertia: Vec<Float>,

    lcg0: si::Meter<Float>,
    lcgf: si::Meter<Float>,
}
