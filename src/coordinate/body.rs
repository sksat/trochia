use crate::types::Float;
use nalgebra::Vector3;

pub struct Body {
    pub x: Float,
    pub y: Float,
    pub z: Float,
}

impl Body {
    fn new(x: Float, y: Float, z: Float) -> Self {
        Self { x, y, z }
    }
}

impl From<Body> for Vector3<Float> {
    fn from(from: Body) -> Vector3<Float> {
        Vector3::<Float>::new(from.x, from.y, from.z)
    }
}

impl From<Vector3<Float>> for Body {
    fn from(from: Vector3<Float>) -> Body {
        Body::new(from[0], from[1], from[2])
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn into() {
        let b = Body::new(1.0, 2.0, 3.0);
        let v: Vector3<Float> = b.into();
        assert_eq!(v, Vector3::<Float>::new(1.0, 2.0, 3.0));
    }
}
