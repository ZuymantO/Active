#CC=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++
CC=g++
C=gcc
#FLAGS=-W -Wall -ansi -pedantic
FLAGS=-W -Wall -lpthread -ldl -g
OBJ=../../obj
EXE=activebi
CMM=../common
# Repertoire de fichier commun
SRC=SQLite3DB.cpp SQLQuery.cpp SQLite3DBException.cpp ActiveBI.cpp

all: make_common $(EXE)

make_common: common.h
	cd $(CMM) ; make
	cd ../bdd/

$(EXE): sqlite3.o SQLite3DBException.o $(CMM)/AQuery.o $(CMM)/AnyFile.o $(CMM)/AudioFile.o $(CMM)/ObjectFile.o $(CMM)/ArchiveFile.o $(CMM)/VideoFile.o $(CMM)/ImageFile.o $(CMM)/TextFile.o SQLite3DB.o SQLQuery.o ActiveBI.o 
	$(CC) $(FLAGS) -pthread $^ -o $(EXE)
	cp $(EXE) ../../bin

sqlite3.o: sqlite3.c sqlite3.h
	 $(C) $(FLAGS) -c $^ -lsqlite3
SQLite3DB.o: SQLite3DB.cpp SQLite3DB.h
	$(CC) $(FLAGS) -c $^ -lsqlite3

SQLite3DBException.o: SQLite3DBException.cpp SQLite3DBException.h
	$(CC) $(FLAGS) -c $^ 

SQLQuery.o: SQLQuery.cpp SQLQuery.h SQLite3DBException.h
	$(CC) $(FLAGS) -c $^ -lsqlite3

ActiveBI.o: ActiveBI.cpp 
	$(CC) $(FLAGS) -c $^ 

clean:
	rm -rf *.o *~ *# *.gch $(GOALS)
	rm -rf $(EXE)
