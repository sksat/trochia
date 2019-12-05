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
