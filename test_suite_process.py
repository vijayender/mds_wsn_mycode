from test_suite_io import *
import os,sys

def run_proc(path, lib, ifile, cfile, ofile,verbose):
    """ Runs test_lib with ifile cfile and ofile
    """
    cmd = "%(p)s/test_%(lib)s -i %(p)s/%(i)s -c  %(p)s/%(c)s -o  %(p)s/%(o)s" % {'p':path, 'lib':lib, 'i':ifile, 'c':cfile, 'o':ofile}
    print "running \""+cmd + "\"  ......."
    if verbose == 1:
        proc = os.popen(cmd)
    else:
        proc = os.popen(cmd+" > /dev/null")
    print proc.read()
    pass

def run_proc_l(path, lib, l_file,verbose=0):
    """ Runs run_proc for each entry in l_file
    """
    print (path+'/'+l_file)
    l_io = test_io_list_in(path+'/'+l_file)
    while True:
        line = l_io.read_entry()
        if len(line)==0:
            break;
        ifile = line.strip()
        ifile = ifile[:-5]      # Strip the existing .data
        run_proc(path, lib, "input/"+ifile+".data", "input/"+ifile+".config_"+lib,
                 "output/"+ifile+"."+lib+".data", verbose)
    pass

if __name__ == "__main__":
    if(len(sys.argv) > 1):
        verbose = int(sys.argv[1])
    else:
        verbose = 0
    print verbose
    run_proc_l('build','basic_sa','input/list1', verbose)
    pass
