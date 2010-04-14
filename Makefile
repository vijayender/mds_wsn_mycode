CC=/usr/bin/gcc -pg -fPIC
CFLAGS=-c -ggdb -Wall -I../annealing/src/lib -I.
LDFLAGS=-lm -ggdb -lgsl -lcblas -lannealing1.1  -L../annealing/build/libraries.d/  -L./build/ 
basedir=.
builddir=$(basedir)/build
LIBRARY_MODE=-shared
#basic_nonsimplex_ldflags = -lbasic_nonsimplex
all: puresa_old basic_nonsimplex

tests: test_basic_nonsimplex test_funcs

test_library: mds.o test_io.o test_library.o
	$(CC) $(LDFLAGS) -lmds_simplex -lbasic_nonsimplex -literative_majorize $(addprefix $(builddir)/,$^) -o build/$@

test_basic_nonsimplex: mds.o test_basic_nonsimplex.o
	$(CC) $(LDFLAGS) -lbasic_nonsimplex $(addprefix $(builddir)/,$^) -o build/$@

test_mds_simplex: mds.o test_mds_simplex.o 
#	$(CC) $(LDFLAGS) $(addprefix $(builddir)/,$^) -o build/$@
	$(CC) $(LDFLAGS) -lmds_simplex $(addprefix $(builddir)/,$^) -o build/$@

test_iterative_majorize: mds.o test_iterative_majorize.o commonFuncs.o 
	$(CC) $(LDFLAGS) -literative_majorize $(addprefix $(builddir)/,$^) -o build/$@

# mds_simplex: mds.o mds_simplex.o commonFuncs.o
# 	$(CC) $(LDFLAGS) $(addprefix $(builddir)/,$^) -o build/$@
#mds_iterative_majorize:  mds.o commonFuncs.o iterative_majorize.o mds_iterative_majorize.o
#	$(CC) $(LDFLAGS) $(LIBRARY_MODE) $(addprefix $(builddir)/,$^) -o build/lib$@.so

iterative_majorize:  mds.o commonFuncs.o iterative_majorize.o
	$(CC) $(LDFLAGS) $(LIBRARY_MODE) $(addprefix $(builddir)/,$^) -o build/lib$@.so


mds_simplex: mds.o mds_simplex.o commonFuncs.o
	$(CC) $(LDFLAGS) $(LIBRARY_MODE) $(addprefix $(builddir)/,$^) -o build/libmds_simplex.so


basic_nonsimplex: mds.o basic_nonsimplex.o
	$(CC) $(LDFLAGS) $(LIBRARY_MODE) $(addprefix $(builddir)/,$^) -o build/libbasic_nonsimplex.so

test_funcs:  test_funcs.o  mds.o test_io.o
	$(CC) $(LDFLAGS)   $(addprefix $(builddir)/,$^) -o build/test_funcs

puresa_old: pureSA.o lossFunctions.o
	$(CC) $(LDFLAGS) $(builddir)/pureSA.o $(builddir)/lossFunctions.o -o build/puresa

%.so: %.c
	$(CC) $(CFLAGS) $< -o $(builddir)/lib$@

check-syntax:
	$(CC) -I../annealing/src/lib/ -o nul -S ${CHK_SOURCES}

.c.o:
	$(CC) $(CFLAGS) $< -o $(builddir)/$@

clean:
	rm build/*
