# Altitude history: trochia vs the measured telemetry and OpenRocket.
# Run after trochia.  Data columns are all "time[s] altitude_AGL[m]"
# (trochia pos.dat is "time east up north" -> use 1:3).
set terminal pngcairo size 1180,560 enhanced font "sans,10"
set output "trajectory.png"

BARO  = "measured-telemetrum.dat"   # baro, full flight to landing
INERT = "measured-inertial.dat"     # IMU/inertial, reliable on ascent
OR    = "openrocket.dat"            # OpenRocket sim
TROC  = "output/nominal/89/5/180/pos.dat"

set multiplot layout 1,2 title "PSAS LV2 (Launch 12): trochia vs measured telemetry vs OpenRocket" font "sans,12"

# --- ascent (validation) ---
set title "Ascent (validation)"
set xlabel "time [s]"; set ylabel "altitude AGL [m]"
set xrange [0:45]; set yrange [0:5300]; set grid
set key bottom right
plot BARO  using 1:2 with lines lw 3 lc rgb "#e08214" title "measured baro", \
     INERT using 1:2 with lines lw 2 lc rgb "#762a83" title "measured inertial", \
     OR    using 1:2 with lines lw 2 lc rgb "#999999" title "OpenRocket", \
     TROC  using 1:3 with lines lw 2 lc rgb "#31688e" title "trochia"

# --- full flight (incl. descent; each tool models a different recovery) ---
set title "Full flight (descent depends on the recovery model)"
set xlabel "time [s]"; set ylabel "altitude AGL [m]"
set xrange [0:760]; set yrange [0:5300]; set grid
set key top right
plot BARO using 1:2 with lines lw 3 lc rgb "#e08214" title "measured baro", \
     OR   using 1:2 with lines lw 2 lc rgb "#999999" title "OpenRocket", \
     TROC using 1:3 with lines lw 2 lc rgb "#31688e" title "trochia (single main)"

unset multiplot
