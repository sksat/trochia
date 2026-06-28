# Apogee vs effective constant thrust (vertical, still air). The measured 113.4 km
# apogee picks the trajectory-matched effective thrust ~13.9 kN -- well above the
# 12 kN sea-level nominal (the difference is the pressure-fed altitude thrust gain).
# thrust-apogee.dat ("thrust[kN] apogee[km] t_apogee[s]") is produced by run.sh.
set terminal pngcairo size 780,560 enhanced font "sans,11"
set output "thrust-sweep.png"

set title "MOMO apogee vs effective thrust\nmeasured 113.4 km picks 13.9 kN (sea-level nominal is 12 kN)"
set xlabel "effective constant thrust [kN]"; set ylabel "apogee [km]"
set grid
set key top left

MEAS = 113.4
set arrow from graph 0, first MEAS to graph 1, first MEAS nohead lw 2 dt 2 lc rgb "#e08214"
set label "measured 113.4 km (MOMO-3)" at graph 0.04, first MEAS+6 tc rgb "#b35806"
set arrow from 12.0, graph 0 to 12.0, graph 1 nohead dt 3 lc rgb "#888888"
set label "12 kN\n(sea-level nominal)" at 12.05, 150 left tc rgb "#555555" font "sans,9"
set arrow from 13.9, graph 0 to 13.9, graph 1 nohead dt 3 lc rgb "#31688e"
set label "13.9 kN\n(matched)" at 13.95, 60 left tc rgb "#31688e" font "sans,9"

plot "thrust-apogee.dat" using 1:2 with linespoints pt 7 ps 1.2 lw 2 lc rgb "#31688e" title "trochia (vertical)"
