ODIR	:= obj/
HDIR	:= include/
SDIR	:= src/
BDIR	:= bin/

OBJS_LSH	= lsh_main.o lsh_vector.o myHashTable.o sequence.o utils.o core_utils.o knn.o
OBJS_CL		= lsh_vector.o hcube.o myHashTable.o sequence.o utils.o cluster_main.o cluster.o confs.o core_utils.o knn.o clusterLloyd.o clusterReverse.o
OBJS_CUBE 	= hcube.o hcube_main.o myHashTable.o sequence.o utils.o core_utils.o knn.o
SOURCE		= lsh_main.cpp lsh_vector.cpp myHashTable.cpp sequence.cpp utils.cpp
HEADER		= lsh_vector.h myHashTable.h sequence.h utils.h
OUT			= lsh_vector cluster cube
CC			= g++
FLAGS		= -g -c

_OBJS_LSH = $(patsubst %,$(ODIR)%,$(OBJS_LSH))
_OBJS_CUBE = $(patsubst %,$(ODIR)%,$(OBJS_CUBE))
_OBJS_CL = $(patsubst %,$(ODIR)%,$(OBJS_CL))
_OUT = $(patsubst %,$(BDIR)%,$(OUT))

all: $(OUT)

lsh_main.o: $(SDIR)lsh_main.cpp $(patsubst %,$(HDIR)%,lsh_vector.h sequence.h PQUnique.h PQUnique.t.hpp core_utils.h)
	$(CC) $(FLAGS) $(SDIR)lsh_main.cpp -o $(ODIR)$@

lsh_vector.o: $(SDIR)lsh_vector.cpp $(patsubst %,$(HDIR)%,lsh_vector.h myHashTable.h utils.h PQUnique.h PQUnique.t.hpp)
	$(CC) $(FLAGS) $(SDIR)lsh_vector.cpp -o $(ODIR)$@

myHashTable.o: $(SDIR)myHashTable.cpp $(patsubst %,$(HDIR)%,myHashTable.h utils.h PQUnique.h PQUnique.t.hpp)
	$(CC) $(FLAGS) $(SDIR)myHashTable.cpp -o $(ODIR)$@

sequence.o: $(SDIR)sequence.cpp $(HDIR)sequence.h
	$(CC) $(FLAGS) $(SDIR)sequence.cpp -o $(ODIR)$@

utils.o: $(SDIR)utils.cpp $(HDIR)utils.h
	$(CC) $(FLAGS) $(SDIR)utils.cpp -o $(ODIR)$@

cluster_main.o: $(SDIR)cluster_main.cpp $(patsubst %,$(HDIR)%,cluster.h clusterLloyd.h clusterReverse.h core_utils.h)
	$(CC) $(FLAGS) $(SDIR)cluster_main.cpp -o $(ODIR)$@

cluster.o: $(SDIR)cluster.cpp $(patsubst %,$(HDIR)%,cluster.h utils.h lsh_vector.h hcube.h confs.h)
	$(CC) $(FLAGS) $(SDIR)cluster.cpp -o $(ODIR)$@

confs.o: $(SDIR)confs.cpp $(HDIR)confs.h
	$(CC) $(FLAGS) $(SDIR)confs.cpp -o $(ODIR)$@

hcube_main.o: $(SDIR)hcube_main.cpp $(patsubst %,$(HDIR)%,hcube.h sequence.h core_utils.h)
	$(CC) $(FLAGS) $(SDIR)hcube_main.cpp -o $(ODIR)$@

hcube.o: $(SDIR)hcube.cpp $(patsubst %,$(HDIR)%,hcube.h myHashTable.h utils.h)
	$(CC) $(FLAGS) $(SDIR)hcube.cpp -o $(ODIR)$@

core_utils.o: $(SDIR)core_utils.cpp $(patsubst %,$(HDIR)%,core_utils.h sequence.h PQUnique.h PQUnique.t.hpp)
	$(CC) $(FLAGS) $(SDIR)core_utils.cpp -o $(ODIR)$@

knn.o: $(SDIR)knn.cpp $(HDIR)knn.h
	$(CC) $(FLAGS) $(SDIR)knn.cpp -o $(ODIR)$@

clusterLloyd.o: $(SDIR)clusterLloyd.cpp $(patsubst %,$(HDIR)%,clusterLloyd.h confs.h)
	$(CC) $(FLAGS) $(SDIR)clusterLloyd.cpp -o $(ODIR)$@

clusterReverse.o: $(SDIR)clusterReverse.cpp $(patsubst %,$(HDIR)%,clusterReverse.h lsh_vector.h hcube.h)
	$(CC) $(FLAGS) $(SDIR)clusterReverse.cpp -o $(ODIR)$@

lsh_vector: $(OBJS_LSH)
	$(CC) -g $(_OBJS_LSH) -o $(BDIR)$@

cluster: $(OBJS_CL)
	$(CC) -g $(_OBJS_CL) -o $(BDIR)$@

cube: $(OBJS_CUBE)
	$(CC) -g $(_OBJS_CUBE) -o $(BDIR)$@

clean:
	rm -rf $(_OBJS_CL) $(_OBJS_LSH) $(_OBJS_CUBE) $(_OUT)
