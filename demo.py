from numpy import *
import numpy.linalg.linalg as l
#from cmds import *
import pylab
import sys


def flipColumns(X,i,j):
    """ Flip ith and jth columns of X

    Original X is modified
    
    @returns: X with flipped values

    """
    t = copy(X[:,j])
    X[:,j] = X[:,i]
    X[:,i] = t
    return X

def findRotation(l1,l2):        # x1,x2 are array of coordinates of
                                # point in 2D
    """ Compute the angle of rotation of l2 compared to l1

    @param l1: [[1x2],[1x2]] matrix containing points
    @param l2: same as l1 but in different frame

    Assumes that both the frames are not flipped
    Check with findFlip and flip if necessary
    
    @returns: A matrix of [[c, s],[-s, c]] that rotates a [1x2] point,
    which when multiplied to frame l1 produces l2
    

    """
    x,y = (l1[0,:] - l1[1,:]).A1 # d1,d2 stand for directions
    a,b = (l2[0,:] - l2[1,:]).A1
    # Compute cosine and sine
    c = (x/b+y/a)*a*b/(a**2+b**2) #cosine
    s = (-x/a+y/b)*a*b/(a**2+b**2) #sine
    #return matrix
    return matrix([[c,s],[-s,c]])

    pass

def findTranslation(p1,p2):
    """ 
    Assumes that both frames have same angle and same flip
    
    @returns: The translation point which must be added to get p2 from
    p1

    """

    return p2 - p1

def findFlip(t1, t2): # Each arg is a array of coordinates
    """Check if the provided triangles are flipped

    @param t1: 2x3 matrix consisting of coordinates of triangle in
    frame1
    @param t2: 2x3 matrix consisting of coordinates of triangle in
    frame2
    
    @returns: True if the frames are flipped

    """
    det1 = l.det(concatenate((t1,ones((1,3)))))
    det2 = l.det(concatenate((t2,ones((1,3)))))
    return True if (det1/det2 < 0) else False
    pass

if __name__ == "__main__":
    numpoints = int(sys.argv[1])
    x = random.random(numpoints)*numpoints #numpoints random points
    y = random.random(numpoints)*numpoints
    X = matrix(zip(x,y))
    # Computation of A matrix
    A = zeros((numpoints,numpoints))
    for i in range(0,numpoints):
        for j in range(0,numpoints):
            dx = X[i,:]-X[j,:]
            A[i,j] = dx*dx.T
    # Invoke a CMDS solver
    print "Starting solver .."
    cmds_solv = CMDS()
    cmds_solv.setA(A)
    print "Processing .."
    X1 = cmds_solv.getXeig()[:,0:2] # Optionall can use .getX to use SVD decomposition
    # Done computing
    print "Done computing, analyzing ..."
    A1 = zeros((numpoints,numpoints))
    for i in range(0,numpoints):
        for j in range(0,numpoints):
            if i == j:
                A1[i,j] = l.norm(X1[i,:]-X1[j,:])**2 - A[i,j]
            else:
                A1[i,j] = (l.norm(X1[i,:]-X1[j,:])**2 - A[i,j]) / A[i,j]
    new =  A1
    print "Some stats: regarding the error in computation", new.max(), new.min(), new.mean(), new.std()

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
    pylab.show()
