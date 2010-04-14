CC=/usr/bin/gcc -pg -fPIC
CFLAGS=-c -ggdb -Wall -I../annealing/src/lib -I.
LDFLAGS=-lm -ggdb -lgsl -lcblas -lannealing1.1  -L../annealing/build/libraries.d/  -L./build/ 
basedir=.
builddir=$(basedir)/build
LIBRARY_MODE=-shared
#basic_sa_ldflags = -lbasic_sa
all: basic_sa mds_simplex iterative_majorize test_library test_basic_sa test_mds_simplex test_iterative_majorize

tests: test_basic_sa test_funcs

test_library: mds.o test_io.o test_library.o
	$(CC) $(LDFLAGS) -lmds_simplex -lbasic_sa -literative_majorize $(addprefix $(builddir)/,$^) -o build/$@

test_basic_sa: mds.o test_basic_sa.o
	$(CC) $(LDFLAGS) -lbasic_sa $(addprefix $(builddir)/,$^) -o build/$@

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


basic_sa: mds.o basic_sa.o
	$(CC) $(LDFLAGS) $(LIBRARY_MODE) $(addprefix $(builddir)/,$^) -o build/libbasic_sa.so

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
