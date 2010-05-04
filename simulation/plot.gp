#!/usr/bin/gnuplot -persist
set term pngcairo font "Arial,10" size 1280,800
set font 
set style fill transparent solid 0.5 noborder
set yrange [0:]
set output 'patwari_basic_output.png'
plot 'pat_basic_sa.out.data' with lines

set output 'patwari_im_output.png'
plot 'pat_im.out.data' with lines

set output 'patwari_simplex_output.png'
plot 'pat_simplex.out.data' with lines

