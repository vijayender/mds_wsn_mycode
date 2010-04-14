from test_suite_io import *
import sys
#also imports numpy as np


#write point files which shall remain same for each libraries test
#write config file for each inp file and each corresponding library inp[num].config_[lib_name]


#increasing number of points
#starting points completely different and random
def generate_files_for_increasing_number_of_points_random_inp(start, end, step, startIndex, dir, list_file):
    """ generates a series of input files
    start: start size
    end: end size
    startIndex: inp[num].data index stands for num
    dir: directory in which to save
    list_file: list file in which the data should be APPENDED
    """
    index = startIndex
    li = test_io_list_out(list_file)
    for i in xrange(start,end):
        for j in xrange(0,step):
            le = 'inp'+ ("%(i)05d" % {'i': index})+'.data'
            f = dir+'/inp'+ ("%(i)05d" % {'i': index})+'.data' 
            c = dir+'/inp'+ ("%(i)05d" % {'i': index})+'.config' 
            index += 1
            print "writing ", index
            if not check_file(f):
                io = test_io_inp(f)
                pts = mds_lib.generate_random_points(i)
                d = mds_lib.d2_from_p(pts)
                io.write_out_points(mds_lib.generate_random_points(i), "x")
                io.write_out_points(d, "d")
                io.write_out_points(pts, "p") # p is the points from which d
                # are generated, it is kept as
                # a reference and is not used
                # by any of the c libraries
                io.close
            li.write_entry(le)     # enter into list file (which lists all the inputs)
            # BASIC_NONSIMPLEX
            if not check_file(c+'_basic_nonsimplex'):
                io = test_io_inp_config(c+'_basic_nonsimplex')
                #max_step, energy_limit, temperature, damping_factor, iters, boltzman_k, temp_min,
                io.write("m 10")      # max step
                io.write("e 0.01")    # energy minimum
                io.write("T 1")       # start temperature
                io.write("d 1.05")    # damping factor
                io.write("i 100")      # iters_max per temp
                io.write("b 1")       # boltzman_constant
                io.write("t 0.0000001") # temp_min
                io.write("s "+str(i)) # size
            # ITERATIVE_MAJORIZE
            suffix = '_iterative_majorize'
            if not check_file(c+suffix):
                io = test_io_inp_config(c+suffix)
                io.write("e 0.01")    # energy minimum
                io.write("i 100")      # iters_max per temp
                io.write("s "+str(i)) # size
            # MDS_SIMPLEX
            suffix = '_mds_simplex'
            if not check_file(c+suffix):
                io = test_io_inp_config(c+suffix)
                io.write("e 0.01")    # energy minimum
                io.write("i 100")      # iters_max per temp
                io.write("m 10");
                io.write("s "+str(i)) # size


if __name__ == "__main__":
    path = sys.argv[1]
    listfile = sys.argv[2]
    generate_files_for_increasing_number_of_points_random_inp(3, 5, 1, 3 , path, listfile)
