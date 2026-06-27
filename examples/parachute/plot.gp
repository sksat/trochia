# Parachute vs ballistic descent. Run after run.sh (or trochia on both configs).
# pos.dat columns: time east up(altitude) north
set terminal pngcairo size 1180,520 enhanced font "sans,10"
set output "comparison.png"

CHUTE = "out-chute/89/4/90/pos.dat"
BALL  = "out-ballistic/89/4/90/pos.dat"

set multiplot layout 1,2 title "trochia parachute vs ballistic descent (same flight)" font "sans,12"

set title "Altitude vs time\nchute descends slowly; ballistic falls fast"
set xlabel "time [s]"; set ylabel "altitude [m]"; set grid
set key top right
plot BALL using 1:3 with lines lw 2 lc rgb "#cc0000" title "ballistic", \
     CHUTE using 1:3 with lines lw 2 lc rgb "#31688e" title "parachute"

set title "Side view: downrange vs altitude\nchute drifts downwind, ballistic lands near the pad"
set xlabel "east (downrange) [m]"; set ylabel "altitude [m]"; set grid
set key top right
plot BALL using 2:3 with lines lw 2 lc rgb "#cc0000" title "ballistic", \
     CHUTE using 2:3 with lines lw 2 lc rgb "#31688e" title "parachute"

unset multiplot
