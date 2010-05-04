#!/usr/bin/gnuplot -persist
set term pngcairo font "Arial,10" size 1280,800
set output 'simul_best.png'
set yrange [0:6e6]
plot 'im.data' using 2:4, 'sa.data' using 8:10, 'simplex.data' using 3:5

set output 'simul_best_cropped.png'
set yrange [0:1e6]
replot
set output 'simul_best_cropped2.png'
set yrange [0:100000]
replot
