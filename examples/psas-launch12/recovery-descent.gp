# Why the zones differ: descent altitude-vs-time, single main vs dual deploy.
# pos.dat: time east up north (col 3 = altitude).
set terminal pngcairo size 860,560 enhanced font "sans,11"
set output "recovery-descent.png"

set title "Single main vs dual deployment: descent of the same flight\ndual deploy spends far less time aloft, so it drifts much less"
set xlabel "time [s]"
set ylabel "altitude AGL [m]"
set xrange [0:760]
set yrange [0:5300]
set grid
set key top right

SM = "output/nominal/89/5/0/pos.dat"          # single main
DD = "output-dualdeploy/89/5/0/pos.dat"        # drogue + main

# mark the main-deployment altitude (450 m AGL)
set arrow from 0,450 to 760,450 nohead dt 3 lc rgb "#888888"
set label "main deploys (450 m)" at 755,620 right tc rgb "#666666"

plot SM using 1:3 with lines lw 2 lc rgb "#cc0000" title "single main (lands 736 s)", \
     DD using 1:3 with lines lw 2 lc rgb "#31688e" title "dual deploy (lands 292 s)"
