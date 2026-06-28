# Why MOMO needs active guidance: the same vehicle, launched dead-vertical vs
# 0.1 deg off vertical, in an UNGUIDED constant-Cd model. With T/W ~ 1.06 the
# rocket leaves the rail at only a few m/s, so the tiny tilt is never corrected
# (no thrust-vector control) -- it weathercocks far over instead of flying
# MOMO's near-vertical guided ascent. Altitude vs ground range; pos.dat is
# "time east up north".
set terminal pngcairo size 900,560 enhanced font "sans,11"
set output "unguided-limit.png"

set title "Unguided model limit: MOMO needs thrust-vector control\nvertical reaches 113 km straight up; 0.1 deg off-vertical pitches over (unguided, no TVC)"
set xlabel "ground range from pad [km]"; set ylabel "altitude [km]"
set yrange [0:125]; set grid
set key top right

VERT = "output/nominal/90/0.001/0/pos.dat"
TILT = "output-tilt/89.9/0.001/0/pos.dat"

set arrow from 0,100 to 160,100 nohead dt 3 lc rgb "#888888"
set label "Karman line 100 km" at 4,103 tc rgb "#888888" font "sans,9"

plot VERT using (sqrt($2**2+$4**2)/1000):($3/1000) with lines lw 2.5 lc rgb "#31688e" \
        title "vertical (90 deg): 113 km", \
     TILT using (sqrt($2**2+$4**2)/1000):($3/1000) with lines lw 2.5 lc rgb "#cc0000" \
        title "89.9 deg unguided: pitches over to 31 km, 147 km downrange"
