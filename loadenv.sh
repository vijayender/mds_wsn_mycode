#!/bin/bash 

export LD_LIBRARY_PATH=~/wsn/mds/code/annealing/build/libraries.d/:$LD_LIBRARY_PATH:.:build/

build/test_basic_nonsimplex $@