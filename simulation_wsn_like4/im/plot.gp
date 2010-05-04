#!/usr/bin/gnuplot -persist
set term pngcairo font "Arial,10" size 1280,800
set font 
set style fill transparent solid 0.5 noborder
set output 'im_2d.png'
#set term x11
set xlabel "iterations per temperature"
set ylabel "No. of nodes"
set zlabel "Time taken"
set title 'Plot of convergence for iterative majorization'
set grid

f(x) = (x < 0.1)?0.5:1

plot "< awk '{if($3<0.1) print}' test.data" using 2:4 with points pt 1 ps 1.5 lc rgb 'blue' ti 'Converged 0.1 points',\
     "< awk '{if($3>0.1 && $3<0.15) print}' test.data" using 2:4 with points pt 7 ps 2 lc rgb 'green' ti 'coverged only to 0.15 points',\
     "< awk '{if($3>0.15) print}' test.data" using 2:4 with points pt 2 ps 3 lc rgb 'red' ti 'Unconverged points'
