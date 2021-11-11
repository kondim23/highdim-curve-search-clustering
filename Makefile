ODIR	:= obj/
HDIR	:= include/
SDIR	:= src/
BDIR	:= bin/

OBJS_LSH	= lsh_main.o lsh.o myHashTable.o point.o utils.o
OBJS_CL		= lsh.o hcube.o myHashTable.o point.o utils.o cluster_main.o cluster.o confs.o
OBJS_CUBE 	= hcube.o hcube_main.o myHashTable.o point.o utils.o
SOURCE		= lsh_main.cpp lsh.cpp myHashTable.cpp point.cpp utils.cpp
HEADER		= lsh.h myHashTable.h point.h utils.h
OUT			= lsh cluster cube
CC			= g++
FLAGS		= -g -c

_OBJS_LSH = $(patsubst %,$(ODIR)%,$(OBJS_LSH))
_OBJS_CUBE = $(patsubst %,$(ODIR)%,$(OBJS_CUBE))
_OBJS_CL = $(patsubst %,$(ODIR)%,$(OBJS_CL))
_OUT = $(patsubst %,$(BDIR)%,$(OUT))

all: $(OUT)

lsh_main.o: $(SDIR)lsh_main.cpp $(patsubst %,$(HDIR)%,lsh.h point.h PQUnique.h PQUnique.t.hpp)
	$(CC) $(FLAGS) $(SDIR)lsh_main.cpp -o $(ODIR)$@

lsh.o: $(SDIR)lsh.cpp $(patsubst %,$(HDIR)%,lsh.h myHashTable.h utils.h PQUnique.h PQUnique.t.hpp)
	$(CC) $(FLAGS) $(SDIR)lsh.cpp -o $(ODIR)$@

myHashTable.o: $(SDIR)myHashTable.cpp $(patsubst %,$(HDIR)%,myHashTable.h utils.h PQUnique.h PQUnique.t.hpp)
	$(CC) $(FLAGS) $(SDIR)myHashTable.cpp -o $(ODIR)$@

point.o: $(SDIR)point.cpp $(HDIR)point.h
	$(CC) $(FLAGS) $(SDIR)point.cpp -o $(ODIR)$@

utils.o: $(SDIR)utils.cpp $(HDIR)utils.h
	$(CC) $(FLAGS) $(SDIR)utils.cpp -o $(ODIR)$@

cluster_main.o: $(SDIR)cluster_main.cpp $(HDIR)cluster.h
	$(CC) $(FLAGS) $(SDIR)cluster_main.cpp -o $(ODIR)$@

cluster.o: $(SDIR)cluster.cpp $(patsubst %,$(HDIR)%,cluster.h utils.h lsh.h)
	$(CC) $(FLAGS) $(SDIR)cluster.cpp -o $(ODIR)$@

confs.o: $(SDIR)confs.cpp $(HDIR)confs.h
	$(CC) $(FLAGS) $(SDIR)confs.cpp -o $(ODIR)$@

hcube_main.o: $(SDIR)hcube_main.cpp $(patsubst %,$(HDIR)%,hcube.h point.h)
	$(CC) $(FLAGS) $(SDIR)hcube_main.cpp -o $(ODIR)$@

hcube.o: $(SDIR)hcube.cpp $(patsubst %,$(HDIR)%,hcube.h myHashTable.h utils.h)
	$(CC) $(FLAGS) $(SDIR)hcube.cpp -o $(ODIR)$@

lsh: $(OBJS_LSH)
	$(CC) -g $(_OBJS_LSH) -o $(BDIR)$@

cluster: $(OBJS_CL)
	$(CC) -g $(_OBJS_CL) -o $(BDIR)$@

cube: $(OBJS_CUBE)
	$(CC) -g $(_OBJS_CUBE) -o $(BDIR)$@

clean:
	rm -rf $(_OBJS_CL) $(_OBJS_LSH) $(_OBJS_CUBE) $(_OUT)
