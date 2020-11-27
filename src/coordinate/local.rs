use super::Local;

use crate::types::Float;
use nalgebra::Vector3;

#[derive(Debug, PartialEq)]
pub struct NED {
    vec: Vector3<Float>,
}

#[derive(Debug, PartialEq)]
pub struct ENU {
    vec: Vector3<Float>,
}

// From impl

impl<T: Local> From<&T> for NED {
    fn from(from: &T) -> Self {
        NED::new(from.north(), from.east(), from.down())
    }
}
impl<T: Local> From<&T> for ENU {
    fn from(from: &T) -> Self {
        ENU::new(from.east(), from.north(), from.up())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn from_ned_enu() {
        let ned = NED::new(1.1, 2.2, 3.3);
        let enu = ENU::from(&ned);
        let ned2 = NED::from(&enu);
        let enu2 = ENU::from(&ned2);
        assert_eq!(ned, ned2);
        assert_eq!(enu, enu2);
    }

    #[test]
    fn into_ned_enu() {
        let ned = NED::new(1.2, 2.3, 3.4);
        let n = &ned;
        let enu: ENU = n.into();
        let ned2: NED = (&enu).into();
        let enu2: ENU = (&ned2).into();
        assert_eq!(ned, ned2);
        assert_eq!(enu, enu2);
    }
}

// struct impl

impl Local for NED {
    fn new(n: Float, e: Float, d: Float) -> Self {
        Self {
            vec: Vector3::<Float>::new(n, e, d),
        }
    }

    fn as_vec(&self) -> Vector3<Float> {
        self.vec
    }

    fn north(&self) -> Float {
        self.vec[0]
    }
    fn east(&self) -> Float {
        self.vec[1]
    }
    fn down(&self) -> Float {
        self.vec[2]
    }

    fn south(&self) -> Float {
        -1.0 * self.north()
    }
    fn west(&self) -> Float {
        -1.0 * self.east()
    }
    fn up(&self) -> Float {
        -1.0 * self.down()
    }
}

impl Local for ENU {
    fn new(e: Float, n: Float, u: Float) -> Self {
        Self {
            vec: Vector3::<Float>::new(e, n, u),
        }
    }

    fn as_vec(&self) -> Vector3<Float> {
        self.vec
    }

    fn east(&self) -> Float {
        self.vec[0]
    }
    fn north(&self) -> Float {
        self.vec[1]
    }
    fn up(&self) -> Float {
        self.vec[2]
    }

    fn west(&self) -> Float {
        -1.0 * self.east()
    }
    fn south(&self) -> Float {
        -1.0 * self.north()
    }
    fn down(&self) -> Float {
        -1.0 * self.up()
    }
}
