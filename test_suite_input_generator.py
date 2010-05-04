from test_suite_io import *
import sys
import getopt
#also imports numpy as np


#write point files which shall remain same for each libraries test
#write config file for each inp file and each corresponding library inp[num].config_[lib_name]

def write_random_points(f, x, d, p, d2=None):
    if not check_file(f):
        io = test_io_inp(f)
        io.write_out_points(x, "x")
        io.write_out_points(p, "p")
        if d2 != None:
            io.write_out_points(d2, "d")
            io.write_out_points(d, "o")
        else:
            io.write_out_points(d, "d")
        # p is the points from which d
        # are generated, it is kept as
        # a reference and is not used
        # by any of the c libraries
        io.close()
    pass

def write_basic_sa_config(f, m, e, T, d, i, b, t, s):
    if not check_file(f+'_basic_sa'):
        io = test_io_inp_config(f+'_basic_sa')
        #max_step, energy_limit, temperature, damping_factor, iters, boltzman_k, temp_min,
        io.write("m "+str(m))      # max step
        io.write("e "+str(e))    # energy minimum
        io.write("T "+str(T))       # start temperature
        io.write("d "+str(d))    # damping factor
        io.write("i "+str(i))      # iters_max per temp
        io.write("b "+str(b))       # boltzman_constant
        io.write("t "+str(t)) # temp_min
        io.write("s "+str(s)) # size
        io.close()


def write_im_config(f, e, i, s):
    if not check_file(f+'_iterative_majorize'):
        io = test_io_inp_config(f+'_iterative_majorize')
        #max_step, energy_limit, temperature, damping_factor, iters, boltzman_k, temp_min,
        io.write("e "+str(e))    # energy minimum
        io.write("i "+str(i))      # iters_max per temp
        io.write("s "+str(s)) # size
        io.close()

def write_simplex_config(f, m, e, i, s):
    if not check_file(f+'_mds_simplex'):
        io = test_io_inp_config(f+'_mds_simplex')
        #max_step, energy_limit, temperature, damping_factor, iters, boltzman_k, temp_min,
        io.write("m "+str(m))
        io.write("e "+str(e))    # energy minimum
        io.write("i "+str(i))      # iters_max per temp
        io.write("s "+str(s)) # size
        io.close()
    
def generate_names(dir, index):
    le = 'inp'+ ("%(i)05d" % {'i': index})+'.data'
    f = dir+'/inp'+ ("%(i)05d" % {'i': index})+'.data' 
    c = dir+'/inp'+ ("%(i)05d" % {'i': index})+'.config' 
    return (le, f, c)

def generate_files_for_simplex(startIndex, dir, list_file):
    index = startIndex
    li = test_io_list_out(list_file)
    e = 0.1
    i = 10000
    for _s in  np.arange(1,100.1, 5):
        s = int(np.floor(_s))
        P = mds_lib.generate_random_points(s)
        D = mds_lib.d2_from_p(P)
        X = mds_lib.generate_random_points(s)
        for m in np.arange(1,100,10):
            le, f, c = generate_names(dir, index)
            index += 1
            print "writing index ", index
            write_random_points(f, X, D, P)
            li.write_entry(le)
            write_simplex_config(c, m, e, i, s)
            print c, m, e, i, s
    li.close()

def generate_files_for_im(startIndex, dir, list_file):
    index = startIndex
    li = test_io_list_out(list_file)
    e = 0.1
    i = 1000
    for _s in  np.arange(1,200.1, .3):
        s = int(np.floor(_s))
        P = mds_lib.generate_random_points(s)
        D = mds_lib.d2_from_p(P)
        X = mds_lib.generate_random_points(s)
        le, f, c = generate_names(dir, index)
        index += 1
        print "writing index ", index
        write_random_points(f, X, D, P)
        li.write_entry(le)
        write_im_config(c, e, i, s)
        print c, e, i, s
    li.close()


def generate_files_for_basic_sa_deep(startIndex, dir, list_file):
    """ generates a series of input files
    startIndex: inp[num].data index stands for num
    dir: directory in which to save
    list_file: list file in which the data should be APPENDED
    """
    index = startIndex
    li = test_io_list_out(list_file)
    e = 0.1
    T = 1
    t = 1e-6
    b = 1
    for s in  xrange(3,101,5):
        P = mds_lib.generate_random_points(s)
        D = mds_lib.d2_from_p(P)
        X = mds_lib.generate_random_points(s)
        for m in np.arange(1,20.1,4):
            for i in xrange(3,101,5):
                for d in (1.05, 1.1, 1.3, 1.5, 2):
                    le, f, c = generate_names(dir, index)
                    index += 1
                    print "writing index ", index
                    write_random_points(f, X, D, P)
                    li.write_entry(le)
                    write_basic_sa_config(c, m, e, T, d, i, b, t, s)
                    print c, m, e, T, d, i, b, t, s
    li.close()

def generate_files_for_simplex_wsn_like(startIndex, dir, list_file):
    index = startIndex
    li = test_io_list_out(list_file)
    e = 0.1
    i = 10000
    for _s in  np.arange(1,100.1, 5):
        s = int(np.floor(_s))
        P = mds_lib.generate_random_points(s)
        D = mds_lib.d2_from_p(P)
        D1 = mds_lib.generate_wsn_like_distances(D, 4)
        X = mds_lib.generate_random_points(s)
        for m in np.arange(1,100,10):
            le, f, c = generate_names(dir, index)
            index += 1
            print "writing index ", index
            write_random_points(f, X, D, P, D1)
            li.write_entry(le)
            write_simplex_config(c, m, e, i, s)
            print c, m, e, i, s
    li.close()

def generate_files_for_im_wsn_like(startIndex, dir, list_file):
    index = startIndex
    li = test_io_list_out(list_file)
    e = 0.1
    i = 1000
    for _s in  np.arange(1,200.1, .3):
        s = int(np.floor(_s))
        P = mds_lib.generate_random_points(s)
        D = mds_lib.d2_from_p(P)
        D1 = mds_lib.generate_wsn_like_distances(D, 4)
        X = mds_lib.generate_random_points(s)
        le, f, c = generate_names(dir, index)
        index += 1
        print "writing index ", index
        write_random_points(f, X, D, P, D1)
        li.write_entry(le)
        write_im_config(c, e, i, s)
        print c, e, i, s
    li.close()


def generate_files_for_basic_sa_deep_wsn_like(startIndex, dir, list_file):
    """ generates a series of input files
    startIndex: inp[num].data index stands for num
    dir: directory in which to save
    list_file: list file in which the data should be APPENDED
    """
    index = startIndex
    li = test_io_list_out(list_file)
    e = 0.1
    T = 1
    t = 1e-6
    b = 1
    for s in  xrange(3,101,5):
        P = mds_lib.generate_random_points(s)
        D = mds_lib.d2_from_p(P)
        D1 = mds_lib.generate_wsn_like_distances(D, 4)
        X = mds_lib.generate_random_points(s)
        for m in np.arange(1,20.1,4):
            for i in xrange(3,101,5):
                for d in (1.05, 1.1, 1.3, 1.5, 2):
                    le, f, c = generate_names(dir, index)
                    index += 1
                    print "writing index ", index
                    write_random_points(f, X, D, P, D1)
                    li.write_entry(le)
                    write_basic_sa_config(c, m, e, T, d, i, b, t, s)
                    print c, m, e, T, d, i, b, t, s
    li.close()


#increasing number of points
#starting points completely different and random
# - m 1:4:20    maximum step
# - i 10:10:100 iterations
# - s 3:5:100   No. of nodes
# - e 0.1       energy limit
# - T 1         Start temperature
# - t 1e-6      minimum temperature
#   We are setting a low value of t since we want the algorithm to converge
# - d 1.05,1.1,1.3,1.5,2      damping constant
# - b 1         boltzman constant
def generate_files_for_basic_sa(startIndex, dir, list_file):
    """ generates a series of input files
    startIndex: inp[num].data index stands for num
    dir: directory in which to save
    list_file: list file in which the data should be APPENDED
    """
    index = startIndex
    li = test_io_list_out(list_file)
    e = 0.1
    T = 1
    t = 1e-6
    b = 1
    for s in  xrange(3,101,10):
        P = mds_lib.generate_random_points(s)
        D = mds_lib.d2_from_p(P)
        X = mds_lib.generate_random_points(s)
        for m in np.arange(1,20.1,4):
            for i in xrange(10,101,10):
                for d in (1.05, 1.1, 1.3, 1.5, 2):
                    le, f, c = generate_names(dir, index)
                    index += 1
                    print "writing index ", index
                    write_random_points(f, X, D, P)
                    li.write_entry(le)
                    write_basic_sa_config(c, m, e, T, d, i, b, t, s)
                    print c, m, e, T, d, i, b, t, s
    li.close()

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
            # BASIC_SA
            if not check_file(c+'_basic_sa'):
                io = test_io_inp_config(c+'_basic_sa')
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

def sum_lower_tr(d):
    sum = 0
    for i in range(0,d.shape[0]):
        for j in range(0,i):
            sum += d[i,j]
    return sum

if __name__ == "__main__":
    if(len(sys.argv) < 7) :
        print "Args needed: -p path -l listfile -a algorithm_number"
        print "algo: 1 -> basic_sa, 2-> im, 3-> simplex, 4-> sa_deep, 5-> im_wsn_like"
        print "      6-> simplex_wsn_like, 7-> sa_deep_wsn_like"
        exit()
    optlist,args = getopt.getopt(sys.argv[1:], 'p:l:a:')
    for key, elem in optlist:
        if key == '-p':
            path = elem
        elif key == '-a':
            algo = elem
        elif key == '-l':
            listfile = elem
        pass
    pass
    print algo
    if algo == "1":
        generate_files_for_basic_sa(3, path, listfile)
    if algo == "2":
        generate_files_for_im(3, path, listfile)
    if algo == "3":
        generate_files_for_simplex(3, path, listfile)
    if algo == "4":
        generate_files_for_basic_sa_deep(3, path, listfile)
    if algo == "5":
        generate_files_for_im_wsn_like(3, path, listfile)
    if algo == "6":
        generate_files_for_simplex_wsn_like(3, path, listfile)
    if algo == "7":
        generate_files_for_basic_sa_deep_wsn_like(3, path, listfile)
    if algo == "8":
        P = mds_lib.generate_random_points(10)
        D = mds_lib.d2_from_p(P)
        D1 = mds_lib.generate_wsn_like_distances(D, 4)
        X = mds_lib.generate_random_points(10)
        dx = mds_lib.d2_from_p(X)
        print P,"P"
        print D,"D"
        print D1,"D1"
        print X,"X"
        print dx-D
        print abs(dx-D)
        print sum_lower_tr(abs(dx-D))/sum_lower_tr(D)
    #generate_files_for_increasing_number_of_points_random_inp(3, 5, 1, 3 , path, listfile)
