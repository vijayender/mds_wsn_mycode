#from cmds import *
from numpy import *
#from demo import *
import pylab
import numpy as np
import scipy.io.matlab.mio as m
import numpy.linalg.linalg as l
import operator
from ctypes import *
from demo import *
from mds_lib import *
from test_suite_io import *
import sys


b = m.loadmat('/home/vijayender/wsn/patwari/patwari.mat')

#b['deviceLocs']
#b.keys()
X = matrix(b['deviceLocs'])

def mds(X,D, energy_limit=1e-1, iters=1000, verbose_mode=1):
    p = convertTo_c_pointer_pointer_array(X)
    d = convertTo_c_pointer_pointer_array(D)

    print "input matrix of distances ------- "
    for i in xrange(0,X.shape[0]):
        for j in xrange(0,X.shape[1]):
            print p[i][j],
        print
    print p, byref(p.contents), p.contents
    print
    loss = c_float(0.0)
    libbasic_sa = CDLL('libiterative_majorize.so')
    libbasic_sa.iterative_majorize_solve.argtypes = [p_t, c_int, c_int, p_t, c_int,  c_float, c_int, POINTER(c_float)];
    a = libbasic_sa.iterative_majorize_solve(p, X.shape[0], X.shape[1], d, iters,
                                  energy_limit, verbose_mode, byref(loss))
    return convertTo_array_from_c_pointer_pointer(p,X.shape[0],X.shape[1])


def get_distance(i,j): #Computes distance between i,j-th points
    return l.norm(b['deviceLocs'][i] - b['deviceLocs'][j])

def get_pdB(i,j):
    return b['P_dB_RSS'][i][j]

def get_distance2(i,j):
    return 1.143*power(10,(-40.5-get_pdB(i,j))/10/2.11)

A = zeros((44,44))
for i in xrange(0,44):
    for j in xrange(0,44):
        A[i,j] = get_distance2(i,j)**2
# A = matrix([[0,1599,2112,2065,1401,912,474],
#             [1599,0,635,568,699,1155,1306],
#             [2112,635,0,325,1252,1733,1909],
#             [2065,568,325,0,986,1496,1879],
#             [1401,699,1252,986,0,562,1365],
#             [912,1155,1733,1496,562,0,1197],
#             [474,1306,1909,1879,1365,1197,0]])


f = sys.argv[1]

X = random.normal(2,3,44*2).reshape((44,2))

io = test_io_inp(f)
io.write_out_points(X, "x")
io.write_out_points(A, "d")
io.close()
