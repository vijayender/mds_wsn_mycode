import numpy as np
import mds_lib
import os

class test_io_list_out:
    """ For writing list files
    """
    def __init__(self, list_file):
        self.list_file = open(list_file,'a')
 
    def write_entry(self, fname):
        line = fname + "\n"
        self.list_file.write(line)

class test_io_list_in:
    """ For reading list files
    """
    def __init__(self, list_file):
        self.list_file = open(list_file,'r')

    def read_entry(self):
        return self.list_file.readline()
            
def check_file(fname):
    return os.path.isfile(fname)

class test_io_inp_config:
    """ For writing config files
    """
    def __init__(self, config_f):
        self.f = open(config_f,'w')

    def write(self,str):
        self.f.write(str+"\n")

    def close(self):
        self.f.close()

class test_io_inp:
    """ For writing inp[num].data and inp[num].config
    """
    def __init__(self, input_file='input.dat'):
        self.input_file = open(input_file,'w')

    def set_points(self, points):
        self.points = points

    def close(self):
        self.input_file.close()

    def write_out_points(self, points, str):
        line = str+("\nl %(x)d %(y)d \n" % {'x':points.shape[0], 'y':points.shape[1]})
        self.input_file.write(line)
        for pt in points:
            # comma separated coordinates
            # newline separated points
            line = ''
            for i in pt:
                line += "%(x)f," % {'x':i}
            line = line[:-1] + "\n"
            self.input_file.write(line)

if __name__ == "__main__":
    io = test_io_inp('build/input.data')
    pts = mds_lib.generate_random_points()
    d = mds_lib.d2_from_p(pts)
    io.write_out_points(pts, "x")
    io.write_out_points(d, "d")
    io.close
