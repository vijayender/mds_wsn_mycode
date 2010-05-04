#!/usr/bin/gnuplot -persist
set term pngcairo font "Arial,10" size 1280,800
set font 
set style fill transparent solid 0.5 noborder
set xlabel "iterations per temperature"
set ylabel "No. of nodes"
set zlabel "Time taken"
set title 'Plot of all points with max-step=1.0'
set grid
set ticslevel 0
set zrange [0:7e6]
set ytics 3,10,100
set xtics 10,10,100
set output 'basic_sa_simall.png'
a =  "<awk '{if($1 != \"#\")if($1==1.0 && ($8 < 101)) print}' test.data"
splot \
      a every 5::4\
      using 2:8:10:($9*5) with points pt 7 ps variable lc rgb "yellow" ti "Damping constant 2",\
      a every 5::3\
      using 2:8:10:($9*5) with points pt 7 ps variable lc rgb "pink" ti "Damping constant 1.5",\
      a every 5::2\
      using 2:8:10:($9*5) with points pt 7 ps variable lc rgb "blue" ti "Damping constant 1.3",\
      a every 5::1\
      using 2:8:10:($9*5) with points pt 7 ps variable lc rgb "green" ti "Damping constant 1.1",\
      a every 5::0\
      using 2:8:10:($9*5) with points pt 7 ps variable lc rgb "red" ti "Damping constant 1.05",\
      a every 5::4\
      using 2:8:10 with impulses lc rgb "yellow" noti,\
      a every 5::3\
      using 2:8:10 with impulses lc rgb "pink" noti,\
      a every 5::2\
      using 2:8:10 with impulses lc rgb "blue" noti,\
      a every 5::1\
      using 2:8:10 with impulses lc rgb "green" noti,\
      a every 5::0\
      using 2:8:10 with impulses lc rgb "red" noti
