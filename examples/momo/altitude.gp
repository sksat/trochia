# MOMO vertical altitude history: nominal flight + abort energy envelope,
# vs the real MOMO flight apogees. trochia pos.dat is "time east up north"
# (altitude = column 3); measured.dat is "t_apogee[s] apogee[km] label".
set terminal pngcairo size 900,620 enhanced font "sans,11"
set output "altitude.png"

set title "MOMO (Interstellar Technologies): trochia vertical flight + abort energy envelope\nvs the real MOMO-1..4 apogees (effective thrust calibrated to MOMO-3)"
set xlabel "time [s]"; set ylabel "altitude [km]"
set xrange [0:320]; set yrange [0:125]; set grid
set key top left

NOM = "output/nominal/90/0.001/0/pos.dat"
C3  = "output/abort-cato-3s/90/0.001/0/pos.dat"
C64 = "output/abort-cutoff-64s/90/0.001/0/pos.dat"
B70 = "output/abort-breakup-70s/90/0.001/0/pos.dat"

set label "Karman line 100 km" at 250,103 right tc rgb "#888888" font "sans,9"
set arrow from 0,100 to 320,100 nohead dt 3 lc rgb "#888888"

plot NOM using 1:($3/1000) with lines lw 2.5 lc rgb "#31688e" title "nominal (MOMO-3)", \
     B70 using 1:($3/1000) with lines lw 2 lc rgb "#dd8800" title "engine stop 70 s (MOMO-1-like)", \
     C64 using 1:($3/1000) with lines lw 2 lc rgb "#a05000" title "engine stop 64 s (MOMO-4-like)", \
     C3  using 1:($3/1000) with lines lw 2 lc rgb "#cc0000" title "CATO 3 s (MOMO-2-like)", \
     "measured.dat" using 1:2 with points pt 7 ps 1.6 lc rgb "#000000" title "real MOMO apogee", \
     "measured.dat" using 1:2:3 with labels offset 0,1.2 font "sans,8.5" notitle
