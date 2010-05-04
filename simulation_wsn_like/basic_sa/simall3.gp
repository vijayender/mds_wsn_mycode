#!/usr/bin/gnuplot -persist
set term pngcairo font "Arial,10" size 1280,800
set font 
set style fill transparent solid 0.5 noborder
set output 'basic_sa_deep_2d_simall_best_wl.png'
#set term x11
set xlabel "iterations per temperature"
set ylabel "No. of nodes"
set zlabel "Time taken"
set title 'Plot of best cases'
set grid
set ticslevel 0
set zrange [0:1.4e6]
set ytics 3,5,100
set xtics 3,5,100
a =  "test2.data"
a5 =  "<awk '{if($1 != \"#\")if($1==17.0 && ($8 < 101)) print}' test2.data"
a4 =  "<awk '{if($1 != \"#\")if($1==13.0 && ($8 < 101)) print}' test2.data"
a3 =  "<awk '{if($1 != \"#\")if($1==9.0 && ($8 < 101)) print}' test2.data"
a2 =  "<awk '{if($1 != \"#\")if($1==5.0 && ($8 < 101)) print}' test2.data"
a1 =  "<awk '{if($1 != \"#\")if($1==1.0 && ($8 < 101)) print}' test2.data"
#      'test.data' using 2:8:10 with impulses lc rgb "yellow", a5
f(x) = log(x)/5
plot \
      a5 \
       using 2:8:(f($10)):($9*10) with points pt 7 ps variable lc rgb "yellow" ti "Damping constant 2",\
      a4 \
      using 2:8:(f($10)):($9*10) with points pt 7 ps variable lc rgb "pink" ti "Damping constant 1.5",\
      a3 \
      using 2:8:(f($10)):($9*10) with points pt 7 ps variable lc rgb "blue" ti "Damping constant 1.3",\
      a2 \
      using 2:8:(f($10)):($9*10) with points pt 7 ps variable lc rgb "green" ti "Damping constant 1.1",\
      a1 \
      using 2:8:(f($10)):($9*10) with points pt 7 ps variable lc rgb "red" ti "Damping constant 1.05",\
      a using 2:8:1 with labels
#pause -1