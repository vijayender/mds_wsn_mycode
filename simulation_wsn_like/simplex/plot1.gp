#!/usr/bin/gnuplot -persist
set term pngcairo font "Arial,10" size 1280,800
set font 
set style fill transparent solid 0.5 noborder
set output 'simplex_2d.png'
#set term x11
set xlabel "Max-step"
set ylabel "No. of nodes"
set zlabel "Time taken"
set title 'Plot of simplex minimization'
set grid
set ticslevel 0
f(x) = (x < 0.2)?0.5:1

splot "test.data" using 2:3:5:($4*10) with points pt 7 ps variable lc rgb 'blue' ti 'Converged points',\
      "test.data" using 2:3:5:($4*10) with impulses noti
 
