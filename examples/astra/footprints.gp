# Contingency landing footprints (one loop per scenario), East-North from the pad.
# ghp.csv columns: wspeed, wdir, ghp_e, ghp_n, max_altitude.
set datafile separator ","
set terminal pngcairo size 820,800 enhanced font "sans,11"
set output "footprints.png"
set title "Astra contingency landing footprints (落下分散)\nelev 84 deg, heading 133 deg, 5 m/s wind swept 0-360 deg"
set xlabel "East [m]"; set ylabel "North [m]"
set size ratio -1; set grid
set key box opaque top left
set xzeroaxis lt -1 lw 0.5; set yzeroaxis lt -1 lw 0.5

N="output/nominal/84/ghp.csv"
F="output/recovery-failure/84/ghp.csv"
M="output/motor-cutoff/84/ghp.csv"
C="output/cato/84/ghp.csv"

$LAUNCH << EOD
0,0
EOD

# downrange direction (heading 133 deg): E=sin, N=cos
set arrow from 0,0 to 900*sin(133*pi/180),900*cos(133*pi/180) \
    head filled lw 2 lc rgb "#666666"
set label "downrange\n(133 deg)" at 760*sin(133*pi/180),760*cos(133*pi/180) \
    left tc rgb "#666666"

plot N using 3:4 skip 1 with linespoints pt 7 ps 0.5 lc rgb "#31688e" title "nominal (dual deploy)", \
     F using 3:4 skip 1 with linespoints pt 7 ps 0.5 lc rgb "#cc0000" title "recovery failure (ballistic)", \
     M using 3:4 skip 1 with linespoints pt 7 ps 0.5 lc rgb "#dd8800" title "motor cutoff @ 1.5 s", \
     C using 3:4 skip 1 with linespoints pt 7 ps 0.5 lc rgb "#4ac16d" title "CATO @ 1.0 s", \
     $LAUNCH using 1:2 with points pt 3 ps 2.5 lw 2 lc rgb "black" title "launch"
