OBJS	= lsh_main.o lsh.o myHashTable.o point.o utils.o
SOURCE	= lsh_main.cpp lsh.cpp myHashTable.cpp point.cpp utils.cpp
HEADER	= lsh.h myHashTable.h point.h utils.h
OUT	= lsh
CC	= g++
FLAGS	= -g -c

all: $(OUT)

lsh_main.o: lsh_main.cpp lsh.h point.h PQUnique.h PQUnique.t.hpp
	$(CC) $(FLAGS) lsh_main.cpp

lsh.o: lsh.cpp lsh.h myHashTable.h utils.h PQUnique.h PQUnique.t.hpp
	$(CC) $(FLAGS) lsh.cpp

myHashTable.o: myHashTable.cpp myHashTable.h utils.h PQUnique.h PQUnique.t.hpp
	$(CC) $(FLAGS) myHashTable.cpp

point.o: point.cpp point.h
	$(CC) $(FLAGS) point.cpp

utils.o: utils.cpp utils.h
	$(CC) $(FLAGS) utils.cpp

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

clean:
	rm -f $(OBJS) $(OUT)
