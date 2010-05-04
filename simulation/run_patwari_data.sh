#!/bin/bash

../build/test_library -i patwari_inp.data -c basic.config_mds_simplex -o patwari_out$1.simplex.data -l mds_simplex -v | grep --color=never 'f(x)' | cut -d' ' -f2 > pat_simplex.out$1.data
../build/test_library -i patwari_inp.data -c basic.config_iterative_majorize -o patwari_out$1.im.data -l iterative_majorize -v | grep --color=never loss_temp | awk '{print $3}' > pat_im.out$1.data
../build/test_library -i patwari_inp.data -c basic.config_basic_sa -o patwari_out$1.basic_sa.data -l basic_sa -v | grep current | awk '{print substr($4,0,length($4)-2)}' > pat_basic_sa.out$1.data
