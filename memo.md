# 座標系について

## 機体座標系

- ランチャ仰角0度の状態で定義する
	- 右手座標系
	- 原点:		重心
	- X軸:		ノーズ方向を正，エンジン方向を負
	- Y軸:		X軸を右に90度回転させた方向を正(なんか物が置いてあっても良い方向)
	- Z軸:		鉛直下向きを正
		- このとき，X軸を北に向けるとX,Y,Z成分がNEDと対応する
	- ロール角:	X軸周りの回転角
	- ピッチ角:	Y軸周りの回転角
	- ヨー角:	Z軸周りの回転角
		- ランチャ仰角がピッチ角と対応する

## 大気モデル

### ISA: International Standard Atmosphere

- [ISO規格](https://www.iso.org/standard/7472.html)
- [JIS(ISOの翻訳)](http://kikakurui.com/w/W0201-1990-01.html)

### USSA1976: U.S. Standard Atmosphere 1976

- -5km〜86km
- [元論文](https://ntrs.nasa.gov/search.jsp?R=19770009539)

#### 実装例

- [PDAS](https://github.com/jgoppert/pdas/blob/master/atmos/ussa1976.dpr)
- [1976 Standard Atmosphere Calculator](https://www.digitaldutch.com/atmoscalc/index.htm)
- [cl-ussa1976](https://github.com/mmullins/cl-ussa1976)

## 空気抵抗

- ρ: 大気密度
- S: 基準面積(代表面積)
- Va: 対気速度(ベクトル)
	空気に対する機体の速度ベクトル．
	Vwind - Vrocket
- α: 迎角(angle of attack)
	機体が流れに対してどれだけ傾いているか．
	tan(α) = Va.x / Va.z
	α = atan(Va.x / Va.z)
- β: 横滑り角(angle of side slip)
	sin(β) = Va.y / norm(Va)
	β = asin(Va.y / norm(Va)
- q: 動圧
	0.5 * ρ * norm(Va)^2
	圧力と同じ次元を持つ
- D: 空気抵抗(drag force)
	空気力の合力Aの機体座標x軸のマイナス方向成分(軸力Xとも)
	q * S * Cd
- N: 法線力
	空気力の合力Aの機体座標z軸のマイナス方向成分(航空機でいう揚力)
	q * S * Cl
- C\_D: 空気抵抗係数(軸力係数)
- C\_N: 法線力係数(揚力係数)
- C\_Nα: 法線力係数傾斜(揚力傾斜)
	C\_N / α[rad]


- C\_D, C\_Nは物体の形状及び迎角αの関数
	- だから条件仰角少しずつずらしてCFDする
	- 大きさは関係無い(相似でも同じ)
	- だから風洞が有用


### 参考

- ロケットシステム付録B,C
- [航空機の運動方程式](https://www.flight.t.u-tokyo.ac.jp/~tsuchiya/FlightDynamics/12Motion.pdf)
