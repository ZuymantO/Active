#GOALS=Search
GOALS=Result
CC=g++
CFLAGS=-W -Wall -ansi -pedantic
LDFLAGS=-ltinyxml

all : $(GOALS)

$(GOALS) : $(GOALS).o MyDate.o
	$(CC) -o $(GOALS) $(GOALS).o MyDate.o $(LDFLAGS)

MyDate.o : MyDate.cpp
	$(CC) -o MyDate.o -c MyDate.cpp $(CFLAGS)

$(GOALS).o : $(GOALS).cpp MyDate.cpp
	$(CC) -o $(GOALS).o -c $(GOALS).cpp $(CFLAGS)

clean :
	rm -rf *.o $(GOALS) *~
