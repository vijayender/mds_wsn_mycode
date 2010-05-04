#!/usr/bin/gnuplot -persist
set term pngcairo font "Arial,10" size 1280,800
set font 
set output 'basic_sa_simall_1.png'
set style fill transparent solid 0.5 noborder
set xlabel "iterations per temperature"
set ylabel "No. of nodes"
set zlabel "Time taken"
set title 'Partial plot for damping constant=1.0
set grid
set ticslevel 0
set zrange [0:7e6]
set ytics 3,10,100
set xtics 10,10,100
a =  "<awk '{if($1 != \"#\")if($1==1.0 && ($8 < 101)) print}' test.data"
splot \
      a every 5::1\
      using 2:8:10:($9*10) with points pt 7 ps variable lc rgb "red" ti "Damping constant 1.1",\
      a every 5::0\
      using 2:8:10:($9*10) with points pt 7 ps variable lc rgb "green" ti "Damping constant 1.05",\
      a every 5::1\
      using 2:8:10 with impulses lc rgb "red" noti,\
      a every 5::0\
      using 2:8:10 with impulses lc rgb "green" noti
#pause -1
