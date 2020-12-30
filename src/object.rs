extern crate dimensioned as dim;

use crate::types::Float;

use dim::si;

pub trait Object {
    fn weight(&self) -> si::Kilogram<Float>;
}
