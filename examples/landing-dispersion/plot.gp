# Plot the landing dispersion. Run after trochia, from this directory:
#   gnuplot plot.gp
#
# ghp.csv columns: wspeed, wdir, ghp_e, ghp_n, max_altitude
# One loop per wind speed (wind directions 0..360 deg in order; 360==0 closes it).
set datafile separator ","
set terminal pngcairo size 720,680 enhanced font "sans,10"
set output "dispersion.png"

set title "trochia landing dispersion (GHP)\nelev 72 deg, wind 1-7 m/s x dir every 15 deg"
set xlabel "East [m]"
set ylabel "North [m]"
set size ratio -1
set grid
set key box opaque title "wind"
set xzeroaxis lt -1 lw 0.5
set yzeroaxis lt -1 lw 0.5

# launch point (space-separated would clash with the comma separator above)
$LAUNCH << EOD
0,0
EOD

GHP = "output/72/ghp.csv"
plot for [w in "1 2 3 4 5 6 7"] GHP using (int(column(1))==(w+0) ? column(3) : 1/0):4 \
         skip 1 with lines lw 1.6 title w." m/s", \
     $LAUNCH using 1:2 with points pt 3 ps 2.5 lw 2 lc rgb "red" title "launch"
