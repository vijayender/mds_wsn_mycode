from numpy import *
import sys
from demo import *
import scipy.io.matlab.mio as m
import mds_lib

def read_from_file(f, c):
    inp = open(f,'r')
    line = inp.readline()
    while len(line) > 0:
        if line[0:1] == c:
            print "reading", c,
            line = inp.readline()
            vals = line.split()
            x = int(vals[1])
            y = int(vals[2])
            arr = zeros((x,y))
            for i in xrange(0,x):
                line = inp.readline()
                vals = line.split(',')
                for j in xrange(0,y):
                    arr[i,j] = double(vals[j])
            break
        line = inp.readline()
    return arr
    pass

if __name__ == "__main__":
    X1 = matrix(read_from_file(sys.argv[1], 'p'))
    print "average", average(X1[:,0]), ", ", average(X1[:,1])
    print "std", std(X1[:,0]), ", ", std(X1[:,1])
    #pylab.plot(X1[:,0],X1[:,1],'r+', label="Calculated points") #Plot of calculated content
    #pylab.show()
