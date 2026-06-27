# Altitude history: trochia vs Astra's measured flight (RocketPy flight_data).
# measured.dat is "time[s] altitude_AGL[m]"; trochia pos.dat is
# "time east up north" -> altitude is column 3.
set terminal pngcairo size 1180,560 enhanced font "sans,10"
set output "validation.png"

MEAS = "measured.dat"
TROC = "output-validation/84/0.001/0/pos.dat"

set multiplot layout 1,2 title \
  "Astra (Faraday Rocketry UPV, EuRoC'22): trochia vs measured flight" font "sans,13"

# --- ascent (validation) ---
set title "Ascent (validation): apogee 3260 m vs measured 3249 m"
set xlabel "time [s]"; set ylabel "altitude AGL [m]"
set xrange [0:28]; set yrange [0:3500]; set grid
set key bottom right
set arrow from 24,0 to 24,3249 nohead dt 3 lc rgb "#aaaaaa"
set label "measured apogee\n3249 m, t = 24 s" at 3,3300 left tc rgb "#555555"
plot MEAS using 1:2 with points pt 7 ps 0.7 lc rgb "#e08214" title "measured", \
     TROC using 1:3 with lines  lw 2.5     lc rgb "#31688e" title "trochia (Cd=0.55)"

# --- full flight incl. the ballistic / partial-recovery descent ---
unset arrow; unset label
set title "Full flight: as-flown descent (recovery did not fully deploy, approx 100 m/s)"
set xlabel "time [s]"; set ylabel "altitude AGL [m]"
set xrange [0:65]; set yrange [0:3500]; set grid
set key top right
plot MEAS using 1:2 with points pt 7 ps 0.7 lc rgb "#e08214" title "measured", \
     TROC using 1:3 with lines  lw 2.5     lc rgb "#31688e" title "trochia (effective CdA)"

unset multiplot
