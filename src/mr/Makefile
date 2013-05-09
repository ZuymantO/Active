MAIN=XMLParser
CC=g++
CFLAGS=-W -Wall -ansi -pedantic
LIBS=-ltinyxml

all : $(MAIN)

$(MAIN) : $(MAIN).o MyDate.o Search.o Utils.o
	$(CC) -o $(MAIN) $(MAIN).o MyDate.o Search.o Utils.o $(LIBS)

Utils.o : Utils.cpp
	$(CC) -o Utils.o -c Utils.cpp $(CFLAGS)

MyDate.o : MyDate.cpp Utils.cpp
	$(CC) -o MyDate.o -c MyDate.cpp $(CFLAGS)

Search.o : Search.cpp MyDate.cpp Utils.cpp
	$(CC) -o Search.o -c Search.cpp $(CFLAGS)

$(MAIN).o : $(MAIN).cpp Search.cpp MyDate.cpp Utils.cpp
	$(CC) -o $(MAIN).o -c $(MAIN).cpp $(CFLAGS)

clean :
	rm -rf *.o $(MAIN) *~
