# Contingency landing footprints (one loop per scenario), East-North from the pad.
set datafile separator ","
set terminal pngcairo size 780,760 enhanced font "sans,11"
set output "footprints.png"
set title "PSAS LV2 contingency landing footprints\nelev 89 deg, 5 m/s wind swept 0-360 deg"
set xlabel "East [m]"; set ylabel "North [m]"
set size ratio -1; set grid
set key box opaque top left
set xzeroaxis lt -1 lw 0.5; set yzeroaxis lt -1 lw 0.5
N="output/nominal/89/ghp.csv"; F="output/chute-failure/89/ghp.csv"; C="output/cato/89/ghp.csv"
$LAUNCH << EOD
0,0
EOD
plot N using 3:4 skip 1 with linespoints pt 7 ps 0.5 lc rgb "#31688e" title "nominal (chute, single main)", \
     F using 3:4 skip 1 with linespoints pt 7 ps 0.5 lc rgb "#cc0000" title "chute failure (ballistic)", \
     C using 3:4 skip 1 with linespoints pt 7 ps 0.5 lc rgb "#4ac16d" title "CATO at 3 s", \
     $LAUNCH using 1:2 with points pt 3 ps 2.5 lw 2 lc rgb "black" title "launch"
