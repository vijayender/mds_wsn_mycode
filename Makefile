CC=gcc -fPIC
CFLAGS=-c -ggdb -Wall -I../annealing/src/lib -I.
LDFLAGS=-lm -ggdb -lgsl -lcblas -L../annealing/build/libraries.d/ -lannealing1 -L./build/ 
basedir=.
builddir=$(basedir)/build
LIBRARY_MODE=-shared
#basic_nonsimplex_ldflags = -lbasic_nonsimplex
all: puresa_old basic_nonsimplex

tests: test_basic_nonsimplex test_funcs

test_basic_nonsimplex: mds.o test_basic_nonsimplex.o
	$(CC) $(LDFLAGS) -lbasic_nonsimplex $(addprefix $(builddir)/,$^) -o build/$@

test_mds_simplex: mds.o test_mds_simplex.o
	$(CC) $(LDFLAGS) -lmds_simplex $(addprefix $(builddir)/,$^) -o build/$@


mds_simplex: mds.o mds_simplex.o commonFuncs.o
	$(CC) $(LDFLAGS) $(addprefix $(builddir)/,$^) -o build/$@


mds_simplex: mds.o mds_simplex.o
	$(CC) $(LDFLAGS) $(LIBRARY_MODE) $(addprefix $(builddir)/,$^) -o build/libmds_simplex.so


basic_nonsimplex: mds.o basic_nonsimplex.o
	$(CC) $(LDFLAGS) $(LIBRARY_MODE) $(addprefix $(builddir)/,$^) -o build/libbasic_nonsimplex.so

test_funcs:  test_funcs.o  mds.o
	$(CC) $(LDFLAGS)  $(builddir)/test_funcs.o  $(builddir)/mds.o -o build/test_funcs
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