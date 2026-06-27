# Compare trochia's predicted apogee with the measured flight and the paper's
# OpenRocket prediction. Run after trochia, from this directory:
#   gnuplot plot.gp     # -> apogee-comparison.png
#
# trochia's apogee is read straight from the trajectory it just wrote
# (pos.dat columns: time east up north; column 3 = altitude).
set terminal pngcairo size 760,560 enhanced font "sans,11"
set output "apogee-comparison.png"

stats "output/89/0/0/pos.dat" using 3 nooutput
trochia = STATS_max

meas = 39.0          # measured (PerfectFlite Pnut altimeter)
openrocket = 34.4    # paper's OpenRocket prediction

set title "Estes Viking apogee: trochia vs measured vs OpenRocket\nEstes A8-3, all-up 71.6 g (measured flight, Jhanjee / Georgia Tech)"
set ylabel "Apogee [m]"
set yrange [0:46]
set xrange [-0.6:2.6]
set xtics ("measured\n(altimeter)" 0, "OpenRocket\n(paper)" 1, "trochia" 2)
set boxwidth 0.55
set style fill solid 0.85 border -1
set grid ytics
unset key

# reference line at the measured apogee
set arrow from -0.6,meas to 2.6,meas nohead dt 2 lw 2 lc rgb "#cc0000"

plot "<echo '0 39.0'"                       using 1:2 with boxes lc rgb "#9e9e9e", \
     "<echo '1 34.4'"                       using 1:2 with boxes lc rgb "#9e9e9e", \
     sprintf("<echo '2 %f'", trochia)       using 1:2 with boxes lc rgb "#31688e", \
     "<echo '0 39.0'"                       using 1:($2+1.4):(sprintf("%.1f m", $2)) with labels, \
     "<echo '1 34.4'"                       using 1:($2+1.4):(sprintf("%.1f m", $2)) with labels, \
     sprintf("<echo '2 %f'", trochia)       using 1:($2+1.4):(sprintf("%.1f m", $2)) with labels
