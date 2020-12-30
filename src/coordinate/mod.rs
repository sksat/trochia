pub mod body;
pub mod dcm;
pub mod earth;
pub mod local;

use crate::types::Float;
use nalgebra::Vector3;

pub use body::Body;

pub trait Cartesian<D: nalgebra::Dim> {}

pub trait Local {
    //    fn new(e0: Float, e1: Float, e3: Float) -> Self;

    fn as_vec(&self) -> Vector3<Float>;

    fn north(&self) -> Float;
    fn east(&self) -> Float;
    fn west(&self) -> Float;
    fn south(&self) -> Float;

    fn up(&self) -> Float;
    fn down(&self) -> Float;
}

impl Cartesian<nalgebra::U3> for Local {}
