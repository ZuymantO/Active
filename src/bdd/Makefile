#CC=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++
CC=g++
#FLAGS=-Wall
FLAGS=
OBJ=../../obj
EXE=activebi
SRC=SQLite3DB.cpp SQLQuery.cpp SQLite3DBException.cpp ActiveBI.cpp

all: $(EXE)

active: SQLite3DB.o SQLQuery.o SQLite3DBException.o ActiveBI.o
	$(CC) $(FLAGS) -pthread $^ -o $(EXE)
	cp $(EXE) ../../bin

SQLite3DB.o: SQLite3DB.cpp SQLite3DB.h
	$(CC) $(FLAGS) -c $^ -o $@

SQLite3DBException.o: SQLite3DBException.cpp SQLite3DBException.h
	$(CC) $(FLAGS) -c $^ -o $@

SQLQuery.o: SQLiteQuery.cpp SQLiteQuery.h
	$(CC) $(FLAGS) -c $^ -o $@

ActiveBI.o: ActiveBI.cpp 
	$(CC) $(FLAGS) -c $^ -o $@

clean:
	rm -rf *.o *~ *# $(GOALS)
	rm $(EXE)