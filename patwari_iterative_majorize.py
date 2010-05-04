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


b = m.loadmat('/home/vijayender/wsn/patwari/patwari.mat')

#b['deviceLocs']
#b.keys()
X = matrix(b['deviceLocs'])

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

#Square the distances
# for i in range(0,A.shape[0]):
#     for j in range(0,A.shape[1]):
#         A[i,j] = A[i,j]**2
#Invoke CMDS solver
X = random.normal(2,3,44*2).reshape((44,2))
X1 = matrix(mds(X,A))
X = matrix(X)
i = 5
j = 6
k = 7
xi = X[i,:]
xj = X[j,:]
xk = X[k,:]
x1i = X1[i,:]
x1j = X1[j,:]
x1k = X1[k,:]
flip = findFlip(matrix([xi.A1,xj.A1,xk.A1]).T,matrix([x1i.A1,x1j.A1,x1k.A1]).T)
print "flip required" if flip else "flip not required"
if flip:
    flipColumns(X1,0,1)
    x1i = X1[i,:]
    x1j = X1[j,:]
    x1k = X1[k,:]
# Computation of rotation matrix x1 = r*x
r_matrix = findRotation(matrix([xi.A1,xj.A1]),matrix([x1i.A1,x1j.A1]))
X1 = X1*r_matrix
print "Rotating by ", arccos(r_matrix[0,0])*180/pi
x1i = X1[i,:]
translateTo = findTranslation(x1i,xi) # Add the required translation
X1 = X1 + translateTo
print "Translating to ", translateTo

def sum_lt(d):
    Sum = 0
    for i in xrange(0, d.shape[0]):
        for j in xrange(0,i):
            Sum += d[i,j]
    return Sum
print A
print X1[0,:]
D = zeros((44,44))
for i in xrange(0,44):
    for j in xrange(0,44):
        D[i,j] = l.norm(X1[i,:] - X1[j,:])**2
print D
print D-A
a = sum_lt(A-D)
b = sum_lt(A)
c = sum_lt(D)
print "Final loss is", a, b, c, a/b, a/c
    
# for i in range(0,44):
#     pylab.plot(X1[i,0],X1[i,1],'b+')
#     pylab.plot([X1[i,0],X[i,0]],[X1[i,1],X[i,1]],'g-')
#     pylab.plot(X[i,0],X[i,1],'r+')
#     pylab.text(X[i,0],X[i,1],str(i),color='r')
#     pylab.text(X1[i,0],X1[i,1],str(i))

#pylab.savefig('patwariCMDS.png',dpi=72)
#pylab.show()

