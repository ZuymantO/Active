MAIN=XMLParser
CC=g++
CFLAGS=-W -Wall -ansi -pedantic
LIBS=-ltinyxml

all : $(MAIN)

$(MAIN) : $(MAIN).o MyDate.o Result.o Search.o Indexation.o Utils.o
	$(CC) -o $(MAIN) $(MAIN).o MyDate.o Result.o Search.o Indexation.o Utils.o $(LIBS)

Utils.o : Utils.cpp
	$(CC) -o Utils.o -c Utils.cpp $(CFLAGS)

MyDate.o : MyDate.cpp Utils.cpp
	$(CC) -o MyDate.o -c MyDate.cpp $(CFLAGS)

Search.o : Search.cpp MyDate.cpp Utils.cpp
	$(CC) -o Search.o -c Search.cpp $(CFLAGS)

Indexation.o : Indexation.cpp
	$(CC) -o Indexation.o -c Indexation.cpp $(CFLAGS)

Result.o : Result.cpp
	$(CC) -o Result.o -c Result.cpp $(CFLAGS)

$(MAIN).o : $(MAIN).cpp Result.cpp Search.cpp Indexation.cpp MyDate.cpp Utils.cpp
	$(CC) -o $(MAIN).o -c $(MAIN).cpp $(CFLAGS)

clean :
	rm -rf *.o $(MAIN) *~
