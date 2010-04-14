from numpy import *
from ctypes import *
from mds_lib import *
import numpy.linalg.linalg as l
import pylab

def mds(X,D, max_step=16, energy_limit=1e-3, temperature=1, damping_factor=1.05,
        iters=10, boltzman_k=1, temp_min=1e-5, verbose_mode=1):
    p = convertTo_c_pointer_pointer_array(X)
    d = convertTo_c_pointer_pointer_array(D)

    print "input matrix of distances ------- "
    for i in xrange(0,X.shape[0]):
        for j in xrange(0,X.shape[1]):
            print p[i][j],
        print
    print p, byref(p.contents), p.contents
    print
    libbasic_sa = CDLL('libbasic_sa.so')
    libbasic_sa.mds_solve.argtypes = [p_t, c_int, c_int, p_t, c_float, c_float, c_float, c_float, c_int, c_float, c_float, c_int];
    a = libbasic_sa.mds_solve(p, X.shape[0], X.shape[1], d, max_step,
                                  energy_limit, temperature, damping_factor,
                                  iters, boltzman_k, temp_min, verbose_mode)
    return convertTo_array_from_c_pointer_pointer(p,X.shape[0],X.shape[1])

def plot(X, fmt="b-*"):
    pylab.plot(X[:,0], X[:,1], fmt)

if __name__ == "__main__":
    X = random.normal(2,3,20).reshape((10,2))#array([[1,2],[9,4],[5,6]])
    X.shape[0]
        
    D = d2_from_p(X)
    print "\nX  -------------\n",X,"\nD  -----------\n",D
    
    p_r = random.normal(2,3,20).reshape((10,2))

    p_final = mds(p_r, D,iters=100, verbose_mode=0)
    
    # FINAL result d matrix
    print "\n", d2_from_p(p_final) - d2_from_p(X)
    
    plot(p_r,"g+-")
    plot(X,"b*-")
    plot(p_final,"r*-");

    pylab.show()
    
