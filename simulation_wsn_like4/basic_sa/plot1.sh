#!/bin/bash

case $1 in
    1 )
	for i in 1.0 5.0 9.0 13.0 17.0; do
	    echo simall $i
	    sed -e "s/simall/sim$i/g" simall.gp | sed -e "s/=1.0/=$i/g" | gnuplot
	done 
	;;
    2 )
	for i in 1.0 5.0 9.0 13.0 17.0; do
	    echo sim1 $i
	    sed -e "s/simall/sim$i/g" sim1.gp | sed -e "s/=1.0/=$i/g" | gnuplot
	done
	;;
    *)
	echo provide param
esac