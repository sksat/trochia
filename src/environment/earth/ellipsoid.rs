extern crate lazy_static;
use lazy_static::lazy_static;

use crate::types::Float;
use nalgebra::Vector3;

pub struct Ellipsoid {
    pub a: Float,
    pub f_inv: Float,
    pub f: Float,
    pub b: Float,
    pub e: Float,
    pub e2: Float,
}

lazy_static! {
    static ref AIRY_1830: Ellipsoid = Ellipsoid::new(6_377_563.0, 299.33);
    static ref EVEREST_1830: Ellipsoid = Ellipsoid::new(6_377_276.3, 300.80);
    static ref BESSEL_1841: Ellipsoid = Ellipsoid::new(6_377_397.2, 299.15);
    static ref CLARKE_1866: Ellipsoid = Ellipsoid::new(6_378_206.4, 294.98);
    static ref CLARKE_1880: Ellipsoid = Ellipsoid::new(6_378_249.2, 293.47);
    static ref INTERNATIONAL_1924: Ellipsoid = Ellipsoid::new(6_378_388.0, 297.0);
    static ref KRASOVSKY_1924: Ellipsoid = Ellipsoid::new(6_378_245.0, 298.3);
    static ref INTERNATIONAL_ASTRONOMICAL_UNION_1968: Ellipsoid =
        Ellipsoid::new(6_378_160.0, 298.25);
    static ref WGS72: Ellipsoid = Ellipsoid::new(6_378_135.0, 298.26);
    static ref GRS80: Ellipsoid = Ellipsoid::new(6_378_137.0, 298.26);
    static ref WGS84: Ellipsoid = Ellipsoid::new(GRS80.a, 298.25722);
}

impl Ellipsoid {
    pub fn new(a: Float, f_inv: Float) -> Self {
        let f_tmp = 2.0 * f_inv - 1.0;
        Self {
            a,
            f_inv,
            f: 1.0 / f_inv,
            b: a * (f_inv - 1.0) / f_inv,
            e: Float::sqrt(f_tmp) / f_inv,
            e2: f_tmp / (f_inv * f_inv),
        }
    }

    pub fn w(&self, lat: Float) -> Float {
        (1.0 - self.e2 * lat.sin() * lat.sin()).sqrt()
    }

    pub fn n(&self, lat: Float) -> Float {
        self.a / self.w(lat)
    }
}
