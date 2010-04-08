CC=gcc 
CFLAGS=-c -ggdb -Wall -I../annealing/src/lib -I.
LDFLAGS=-lm -ggdb -lgsl -lcblas -L../annealing/build/libraries.d/ -lannealing1
basedir=.
builddir=$(basedir)/build
all: puresa_old test_funcs basic_nonsimplex

basic_nonsimplex: mds.o basic_nonsimplex.o
	$(CC) $(LDFLAGS) $(addprefix $(builddir)/,$^) -o build/basic_nonsimplex

test_funcs:  test_funcs.o  mds.o
	$(CC) $(LDFLAGS)  $(builddir)/test_funcs.o  $(builddir)/mds.o -o build/test_funcs
puresa_old: pureSA.o lossFunctions.o
	$(CC) $(LDFLAGS) $(builddir)/pureSA.o $(builddir)/lossFunctions.o -o build/puresa

check-syntax:
	$(CC) -I../annealing/src/lib/ -o nul -S ${CHK_SOURCES}

.c.o:
	$(CC) $(CFLAGS) $< -o $(builddir)/$@

clean:
	rm build/*