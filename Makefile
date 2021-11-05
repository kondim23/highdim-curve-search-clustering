ODIR	:= obj/
HDIR	:= include/
SDIR	:= src/
BDIR	:= bin/

OBJS	= lsh_main.o lsh.o myHashTable.o point.o utils.o
SOURCE	= lsh_main.cpp lsh.cpp myHashTable.cpp point.cpp utils.cpp
HEADER	= lsh.h myHashTable.h point.h utils.h
OUT	= lsh
CC	= g++
FLAGS	= -g -c

_OBJS = $(patsubst %,$(ODIR)%,$(OBJS))

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

$(OUT): $(OBJS)
	$(CC) -g $(_OBJS) -o $(BDIR)$(OUT)

clean:
	rm -rf $(_OBJS) $(BDIR)$(OUT)
