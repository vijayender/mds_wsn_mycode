from numpy import *
from ctypes import *
import numpy.linalg.linalg as l
import pylab

p_t = POINTER(POINTER(c_float))

def convertTo_c_pointer_pointer_array(x):
    p = p_t()
    p = cast(create_string_buffer('',size=x.shape[0]*sizeof(POINTER(c_float))),p_t)
    for i in xrange(0,x.shape[0]):
        p[i] = cast(create_string_buffer('',size=x.shape[1]*sizeof(c_float)),POINTER(c_float))
        for j in xrange(0,x.shape[1]):
            p[i][j] = c_float(x[i][j])
    return p

def convertTo_array_from_c_pointer_pointer(x,dim1,dim2):
    p = zeros((dim1,dim2))
    for i in xrange(0,dim1):
        for j in xrange(0,dim2):
            p[i][j] = x[i][j]
    return p

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
    libbasic_nonsimplex = CDLL('libbasic_nonsimplex.so')
    libbasic_nonsimplex.mds_solve.argtypes = [p_t, c_int, c_int, p_t, c_float, c_float, c_float, c_float, c_int, c_float, c_float, c_int];
    a = libbasic_nonsimplex.mds_solve(p, X.shape[0], X.shape[1], d, max_step,
                                  energy_limit, temperature, damping_factor,
                                  iters, boltzman_k, temp_min, verbose_mode)
    return convertTo_array_from_c_pointer_pointer(p,3,2)

def d2_from_p(p):
    d2 = zeros((p.shape[0],p.shape[0]))
    for i in xrange(0, p.shape[0]):
        for j in xrange(0, i):
            d2[i,j] = l.norm(p[i,:] - p[j,:])**2
    return d2;

def plot(X, fmt="b-*"):
    pylab.plot(X[:,0], X[:,1], fmt)

if __name__ == "__main__":
    X = array([[1,2],[9,4],[5,6]])
    X.shape[0]
        
    D = d2_from_p(X)
    print "\nX  -------------\n",X,"\nD  -----------\n",D
    
    p_r = random.normal(2,3,6).reshape((3,2))

    p_final = mds(p_r, D, verbose_mode=0)
    
    # FINAL result d matrix
    print "\n", d2_from_p(p_final)
    
    plot(p_r,"g+-")
    plot(X,"b*-")
    plot(p_final,"r*-");

    pylab.show()
    
