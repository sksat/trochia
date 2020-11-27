use crate::environment::earth::ellipsoid;
use crate::types::Float;

// 緯度・軽度・高度
pub struct LLH {
    pub lat: Float,
    pub lon: Float,
    pub height: Float,
}

// ECI: 地球中心慣性座標系
// 基準面: 平均赤道面
pub struct ECI {
    pub x: Float, // 春分点方向
    pub y: Float, // 基準面X軸から90度の方向
    pub z: Float, // 基準面法線方向(北向き)
}

// ECEF: 地心地球固定座標系(測量法では地心直交座標系)
// 基準面: 平均赤道面
// 自転と同期
pub struct ECEF {
    pub x: Float, // 本子午線固定(経度0度)
    pub y: Float, // 基準面X軸から90度の方向
    pub z: Float, // 基準面法線方向(北向き)
    pub elp: ellipsoid::Ellipsoid,
}

impl LLH {
    fn into_ecef(&self, elp: ellipsoid::Ellipsoid) -> ECEF {
        let n_g = 0.0;
        let h = self.height + n_g;
        let n = elp.n(self.lat);
        let tmp_xy = (n + h) * self.lat.cos();
        ECEF {
            x: tmp_xy * self.lon.cos(),
            y: tmp_xy * self.lon.sin(),
            z: (n * (1.0 - elp.e2) + h) * self.lat.sin(),
            elp,
        }
    }
}

impl From<ECEF> for LLH {
    fn from(from: ECEF) -> LLH {
        let elp = &from.elp;
        let p = (from.x * from.x + from.y * from.y).sqrt();
        let mut lat = (from.z / (p * (1.0 - elp.e2))).atan();

        loop {
            let new_lat = from.z / (p - elp.e2 * elp.n(lat) * lat.cos());
            let diff = (new_lat - lat).abs();
            lat = new_lat;
            if diff <= 1.0e-12 {
                break;
            }
        }

        LLH {
            lat,
            lon: (from.y / from.x).atan(),
            height: p / lat.cos() - elp.n(lat),
        }
    }
}
