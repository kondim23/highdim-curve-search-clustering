ODIR	:= obj/
HDIR	:= include/
SDIR	:= src/
BDIR	:= bin/
TDIR	:= test/

OBJS_CL		= point.o lsh_vector.o hcube.o myHashTable.o sequence.o utils.o cluster_main.o fred.o cluster.o confs.o knn.o clusterLloyd.o clusterReverse.o lsh_discrete.o lsh_curve.o curve.o
OBJS_SEARCH	= point.o hcube.o search.o myHashTable.o sequence.o utils.o knn.o lsh_vector.o fred.o lsh_continuous.o lsh_curve.o lsh_discrete.o curve.o
OBJS_TEST	= curve_test.o main_test.o curve.o sequence.o fred.o utils.o lsh_discrete_test.o lsh_discrete.o knn.o lsh_vector.o lsh_curve.o myHashTable.o cluster_test.o confs.o cluster.o clusterLloyd.o point.o
SOURCE		= lsh_main.cpp lsh_vector.cpp myHashTable.cpp sequence.cpp utils.cpp
HEADER		= lsh_vector.h myHashTable.h sequence.h utils.h
OUT			= cluster search test
CC			= g++
FLAGS		= -g -c

_OBJS_LSH = $(patsubst %,$(ODIR)%,$(OBJS_LSH))
_OBJS_SEARCH = $(patsubst %,$(ODIR)%,$(OBJS_SEARCH))
_OBJS_TEST = $(patsubst %,$(ODIR)%,$(OBJS_TEST))
_OBJS_CUBE = $(patsubst %,$(ODIR)%,$(OBJS_CUBE))
_OBJS_CL = $(patsubst %,$(ODIR)%,$(OBJS_CL))
_OUT = $(patsubst %,$(BDIR)%,$(OUT))

all: $(OUT)

test: test

lsh_vector.o: $(SDIR)lsh_vector.cpp $(patsubst %,$(HDIR)%,lsh_vector.h myHashTable.h utils.h PQUnique.h PQUnique.t.hpp)
	$(CC) $(FLAGS) $(SDIR)lsh_vector.cpp -o $(ODIR)$@

myHashTable.o: $(SDIR)myHashTable.cpp $(patsubst %,$(HDIR)%,myHashTable.h utils.h PQUnique.h PQUnique.t.hpp)
	$(CC) $(FLAGS) $(SDIR)myHashTable.cpp -o $(ODIR)$@

sequence.o: $(SDIR)sequence.cpp $(HDIR)sequence.h
	$(CC) $(FLAGS) $(SDIR)sequence.cpp -o $(ODIR)$@

utils.o: $(SDIR)utils.cpp $(HDIR)utils.h
	$(CC) $(FLAGS) $(SDIR)utils.cpp -o $(ODIR)$@

cluster_main.o: $(SDIR)cluster_main.cpp $(patsubst %,$(HDIR)%,cluster.h clusterLloyd.h clusterReverse.h)
	$(CC) $(FLAGS) $(SDIR)cluster_main.cpp -o $(ODIR)$@

cluster.o: $(SDIR)cluster.cpp $(patsubst %,$(HDIR)%,cluster.h utils.h lsh_vector.h hcube.h confs.h)
	$(CC) $(FLAGS) $(SDIR)cluster.cpp -o $(ODIR)$@

confs.o: $(SDIR)confs.cpp $(HDIR)confs.h
	$(CC) $(FLAGS) $(SDIR)confs.cpp -o $(ODIR)$@

curve.o: $(SDIR)curve.cpp $(patsubst %,$(HDIR)%,curve.h lsh_curve.h utils.h)
	$(CC) $(FLAGS) $(SDIR)curve.cpp -o $(ODIR)$@
	
lsh_continuous.o: $(SDIR)lsh_continuous.cpp $(HDIR)lsh_continuous.h
	$(CC) $(FLAGS) $(SDIR)lsh_continuous.cpp -o $(ODIR)$@

lsh_curve.o: $(SDIR)lsh_curve.cpp $(patsubst %,$(HDIR)%,curve.h lsh_curve.h)
	$(CC) $(FLAGS) $(SDIR)lsh_curve.cpp -o $(ODIR)$@

lsh_discrete.o: $(SDIR)lsh_discrete.cpp $(HDIR)lsh_discrete.h
	$(CC) $(FLAGS) $(SDIR)lsh_discrete.cpp -o $(ODIR)$@

point.o: $(SDIR)point.cpp $(patsubst %,$(HDIR)%,point.h utils.h)
	$(CC) $(FLAGS) $(SDIR)point.cpp -o $(ODIR)$@

hcube.o: $(SDIR)hcube.cpp $(patsubst %,$(HDIR)%,hcube.h myHashTable.h utils.h)
	$(CC) $(FLAGS) $(SDIR)hcube.cpp -o $(ODIR)$@

knn.o: $(SDIR)knn.cpp $(HDIR)knn.h
	$(CC) $(FLAGS) $(SDIR)knn.cpp -o $(ODIR)$@

clusterLloyd.o: $(SDIR)clusterLloyd.cpp $(patsubst %,$(HDIR)%,clusterLloyd.h confs.h)
	$(CC) $(FLAGS) $(SDIR)clusterLloyd.cpp -o $(ODIR)$@

clusterReverse.o: $(SDIR)clusterReverse.cpp $(patsubst %,$(HDIR)%,clusterReverse.h lsh_vector.h hcube.h)
	$(CC) $(FLAGS) $(SDIR)clusterReverse.cpp -o $(ODIR)$@

search.o: $(SDIR)search.cpp $(patsubst %,$(HDIR)%,lsh_vector.h hcube.h lsh_continuous.h lsh_discrete.h knn.h PQUnique.h PQUnique.t.hpp point.h curve.h)
	$(CC) $(FLAGS) $(SDIR)search.cpp -o $(ODIR)$@

fred.o: $(SDIR)fred/frechet.cpp $(HDIR)/fred/frechet.hpp
	$(CC) $(FLAGS) $(SDIR)fred/frechet.cpp -o $(ODIR)$@

curve_test.o:	$(TDIR)curve_test.cpp $(TDIR)curve_test.h $(patsubst %,$(HDIR)%,lsh_curve.h curve.h)
	$(CC) $(FLAGS) $(TDIR)curve_test.cpp -o $(ODIR)$@

lsh_discrete_test.o:	$(TDIR)lsh_discrete_test.cpp $(TDIR)lsh_discrete_test.h $(patsubst %,$(HDIR)%,lsh_discrete.h curve.h)
	$(CC) $(FLAGS) $(TDIR)lsh_discrete_test.cpp -o $(ODIR)$@

cluster_test.o:	$(TDIR)cluster_test.cpp $(TDIR)cluster_test.h $(patsubst %,$(HDIR)%,cluster.h curve.h clusterLloyd.h)
	$(CC) $(FLAGS) $(TDIR)cluster_test.cpp -o $(ODIR)$@

main_test.o:	$(TDIR)main_test.cpp
	$(CC) $(FLAGS) $(TDIR)main_test.cpp -o $(ODIR)$@

cluster: $(OBJS_CL)
	$(CC) -g $(_OBJS_CL) -o $(BDIR)$@

search: $(OBJS_SEARCH)
	$(CC) -g $(_OBJS_SEARCH) -o $(BDIR)$@

test:	$(OBJS_TEST)
	$(CC) -g $(_OBJS_TEST) -o $(BDIR)$@ -lcppunit

clean:
	rm -rf $(_OBJS_CL) $(_OBJS_LSH) $(_OBJS_CUBE) $(_OBJS_SEARCH) $(_OBJS_TEST) $(_OUT)
