# Animated parachute-vs-ballistic descent (shows the time difference).
# Run after run.sh (needs out-chute/ and out-ballistic/):  gnuplot anim.gp
set terminal gif animate delay 6 loop 0 optimize size 720,540 font "sans,10"
set output "descent.gif"

BALL  = "out-ballistic/89/4/90/pos.dat"   # time east up north
CHUTE = "out-chute/89/4/90/pos.dat"

set xlabel "east (downrange) [m]"
set ylabel "altitude [m]"
set xrange [-110:520]
set yrange [0:380]
set grid
set key top right

tmax = 74
do for [t=0:tmax] {
	set title sprintf("trochia: parachute vs ballistic descent    t = %2d s", t)
	plot \
		BALL  using (column(1)<=t ? column(2):1/0):3 with lines lw 2 lc rgb "#cc0000" title "ballistic", \
		CHUTE using (column(1)<=t ? column(2):1/0):3 with lines lw 2 lc rgb "#31688e" title "parachute", \
		BALL  using (abs(column(1)-t)<0.06 ? column(2):1/0):3 with points pt 7 ps 1.6 lc rgb "#cc0000" notitle, \
		CHUTE using (abs(column(1)-t)<0.06 ? column(2):1/0):3 with points pt 7 ps 1.6 lc rgb "#31688e" notitle
}
