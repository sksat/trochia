# Apogee sensitivity to the constant drag coefficient.
# cd-apogee.dat ("Cd apogee[m] t_apogee[s]") is produced by run.sh, which sweeps
# Cd in still air. The measured apogee picks out the representative Cd.
set datafile separator whitespace
set terminal pngcairo size 760,560 enhanced font "sans,11"
set output "cd-sensitivity.png"

set title "Astra apogee vs constant Cd\nthe measured apogee picks the effective constant Cd = 0.55"
set xlabel "constant drag coefficient Cd [-]"
set ylabel "apogee [m]"
set grid
set key top right

MEAS = 3249.0
set arrow from graph 0, first MEAS to graph 1, first MEAS nohead lw 2 dt 2 lc rgb "#e08214"
set label "measured 3249 m" at graph 0.03, first MEAS+90 tc rgb "#b35806"
set arrow from 0.55, graph 0 to 0.55, graph 1 nohead dt 3 lc rgb "#888888"
set label "Cd=0.55" at 0.56, graph 0.10 tc rgb "#555555"

plot "cd-apogee.dat" using 1:2 with linespoints pt 7 ps 1.2 lw 2 lc rgb "#31688e" \
       title "trochia (still air)"
