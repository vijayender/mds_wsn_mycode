from numpy import *
import sys
from demo import *
import scipy.io.matlab.mio as m
import mds_lib

b = m.loadmat('/home/vijayender/wsn/patwari/patwari.mat')
X = matrix(b['deviceLocs'])

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
#    print sys.argv[1]
    print sys.argv[1]
#    X = matrix(read_from_file(sys.argv[1], 'x'))
    X1 = matrix(read_from_file(sys.argv[1], 'p'))

    numpoints = X.shape[0]
    # Computation of transformations required
    # consider i,j points
    i = random.randint(0,numpoints)
    j = random.randint(0,numpoints)
    k = random.randint(0,numpoints)
    xi = X[i,:]
    xj = X[j,:]
    xk = X[k,:]
    x1i = X1[i,:]
    x1j = X1[j,:]
    x1k = X1[k,:]
    #Compute the necessary flip
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
    # Plot ...
    # pylab.plot(X[:,0],X[:,1],'ob-') #Plot of original content
    # pylab.plot(X1[:,0],X1[:,1],'ro-') #Plot of calculated content
    pylab.plot(X[:,0],X[:,1],'xb', label="Original points") #Plot of original content
    pylab.plot(X1[:,0],X1[:,1],'r+', label="Calculated points") #Plot of calculated content
    for i in xrange(0,X.shape[0]):
        # pylab.plot(X[i,0], X[i,1],'ro')
        # pylab.plot(X1[i,0], X1[i,1],'b+')
        pylab.plot([X[i,0], X1[i,0]], [X[i,1], X1[i,1]], 'g-')
    pylab.legend()
    # pylab.xlim(-numpoints,numpoints)
    # pylab.ylim(-numpoints,numpoints)
    st = sys.argv[1]
    st = st.split('/')[1]

    pylab.savefig(st.replace('.','-')+'.png',dpi=150)
    pylab.show()



