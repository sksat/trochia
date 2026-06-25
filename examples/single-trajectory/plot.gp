# Plot the single trajectory. Run after trochia, from this directory:
#   gnuplot plot.gp
#
# pos.dat columns: time  east  up(altitude)  north
set terminal pngcairo size 820,520 enhanced font "sans,10"
set output "trajectory.png"

set title "trochia single trajectory — elev 85 deg, wind 3 m/s\nLARKSPUR-XP.300, 5.035 kg"
set xlabel "East (downrange) [m]"
set ylabel "Altitude [m]"
set grid
set key off

POS = "output/85/3/-90/pos.dat"
plot POS using 2:3 with lines lw 2 lc rgb "#31688e"
