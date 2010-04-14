import ctypes as c
from numpy import *
import numpy.linalg.linalg as l



p_t = c.POINTER(c.POINTER(c.c_float))

def convertTo_c_pointer_pointer_array(x):
    p = c.cast(c.create_string_buffer('',size=x.shape[0]*c.sizeof(c.POINTER(c.c_float))),p_t)
    for i in xrange(0,x.shape[0]):
        p[i] = c.cast(c.create_string_buffer('',size=x.shape[1]*c.sizeof(c.c_float)),c.POINTER(c.c_float))
        for j in xrange(0,x.shape[1]):
            p[i][j] = c.c_float(x[i][j])
    return p

def convertTo_array_from_c_pointer_pointer(x,dim1,dim2):
    p = zeros((dim1,dim2))
    for i in xrange(0,dim1):
        for j in xrange(0,dim2):
            p[i][j] = x[i][j]
    return p


def d2_from_p(p):
    d2 = zeros((p.shape[0],p.shape[0]))
    for i in xrange(0, p.shape[0]):
        for j in xrange(0, i):
            d2[i,j] = l.norm(p[i,:] - p[j,:])**2
    return d2;

def generate_random_points(size=10, mu=0, var=10):
    return random.normal (mu, var, size * 2).reshape(size, 2)
