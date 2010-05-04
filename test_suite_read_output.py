from test_suite_io import *
import sys

# This file provides db like access to the input & config & output of a given list_file

# Provide input specification of the type of data in each file
# as of now support to process float arrays

class process_output:
    def __init__(self, cargs, oargs, list_f, path, lib):
        self.path = path
        self.lib = lib
        self.oargs = oargs
        self.cargs = cargs
        self.list_f = list_f
        pass
    
    def get_elem(self, type, line):
        if (type == 'int'):
            return int(line)
        elif (type == 'float'):
            return float(line)
        pass

    def get_data_o(self, ofile):
        d = {}
        f = open(ofile,"r")
        while True:
            line = f.readline()
            if len(line) == 0:
                break;
            for elem, typ, pr in self.oargs:
                if elem == line[0]:
                    d[elem] = self.get_elem(typ, line[2:])
        return d
    
    def get_data_c(self, inp, cfile):
        d = {}
        d['inp'] = inp
        f = open(cfile,"r")
        while True:
            line = f.readline()
            if len(line) == 0:
                break;
            for elem, typ, pr in self.cargs:
                if elem == line[0]:
                    d[elem] = self.get_elem(typ, line[2:])
        return d

    def get_data(self):
        db = []
        l_io = test_io_list_in(self.list_f)
        while True:
            line = l_io.read_entry()
            if len(line)==0:
                break;
            ifile = line.strip()
            ifile = ifile[:-5]      # Strip the existing .data
            cfile = self.path+'/input/'+ifile+'.config_'+self.lib
            ofile = self.path+'/output/'+ifile+'.'+self.lib+'.data'
            if check_file(ofile):
                cdata = self.get_data_c(ifile, cfile)
                odata = self.get_data_o(ofile)
                db.append(dict(cdata.items() + odata.items()))
        return db
    pass


if __name__ == "__main__":
    path = sys.argv[2]
    f = sys.argv[1]
    lib = sys.argv[3]
    if lib == 'mds_simplex':
        po = process_output([('i','int','i'),
                             ('m','float','m'),
                             ('s','int','s')],[('L','float','L'),
                                             ('u','int','\tu'),
                                             ('S','int','S'),
                                             ('I','int','I')],
                             f, path, lib)
    if lib == 'iterative_majorize':
        po = process_output([('i','int','i'),
                             ('s','int','s')],[('L','float','L'),
                                             ('u','int','\tu'),
                                             ('S','int','S'),
                                             ('I','int','I')],
                             f, path, lib)
    if lib == 'basic_sa':
        po = process_output([('m','float','m'),
                             ('i','int','i'),
                             ('e','float','e'),
                             ('T','float','T'),
                             ('d','float','d'),
                             ('b','float','b'),
                             ('t','float','t'),
                             ('s','int','s')],[('L','float','L'),
                                               ('u','int','\tu'),
                                               ('S','int','S'),
                                               ('I','int','I')],
                             f, path, lib)
    #                        'build/input/list1','build', 'basic_sa')
    l = po.get_data()
    print '#',
    for elem,typ,pr in po.cargs:
        print pr,'\t',
    for elem,typ,pr in po.oargs:
        print pr,'\t',
    print
    for i in l:
        for elem,typ,pr in po.cargs:
            print i[elem],'\t',
            pass
        for elem,typ,pr in po.oargs:
            print i[elem],'\t',
            pass
        print
        pass
    pass
    
