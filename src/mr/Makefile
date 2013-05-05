GOALS = MR

all : $(GOALS)

MR : MR.o MyDate.o
	g++ -o MR MR.o MyDate.o

MyDate.o : MyDate.cpp
	g++ -o MyDate.o -c MyDate.cpp -W -Wall -ansi -pedantic

MR.o : MR.cpp MyDate.cpp
	g++ -o MR.o -c MR.cpp -W -Wall -ansi -pedantic

clean :
	rm -rf *.o $(GOALS)
