#CC=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++
CC=g++
#FLAGS=-Wall
FLAGS=
OBJ=../../obj
EXE=active

CMM=../common
BDD_PATH=../bdd
#COMMONS=$(CMM)/AQuery.o $(CMM)/AnyFile.o $(CMM)/AudioFile.o $(CMM)/ObjectFile.o $(CMM)/ArchiveFile.o $(CMM)/VideoFile.o $(CMM)/ImageFile.o $(CMM)/TextFile.o $(CMM)

#COMMONS=$(CMM)/AnyFile.o $(CMM)/ImageFile.o $(CMM)/Result.o $(CMM)/tinyxmlerror.o $(CMM)/VideoFile.o $(CMM)/AQuery.o $(CMM)/Indexation.o $(CMM)/Search.o $(CMM)/tinyxml.o $(CMM)/XMLGeneration.o $(CMM)/ArchiveFile.o $(CMM)/MyDate.o $(CMM)/TextFile.o $(CMM)/tinyxmlparser.o $(CMM)/XMLParser.o $(CMM)/AudioFile.o $(CMM)/ObjectFile.o $(CMM)/tinystr.o $(CMM)/Utils.o
COMMONS=$(CMM)/AnyFile.o $(CMM)/ImageFile.o $(CMM)/Result.o $(CMM)/tinyxmlerror.o $(CMM)/VideoFile.o $(CMM)/AQuery.o $(CMM)/Indexation.o $(CMM)/Search.o $(CMM)/tinyxml.o $(CMM)/XMLGeneration.o $(CMM)/ArchiveFile.o $(CMM)/MyDate.o $(CMM)/TextFile.o $(CMM)/tinyxmlparser.o $(CMM)/XMLParser.o $(CMM)/AudioFile.o $(CMM)/ObjectFile.o $(CMM)/tinystr.o $(CMM)/Utils.o

OBJ=active
GOALS=active example client


all: xml active client example	
	cp active ../../bin
	cp client ../../bin
	cp example ../../bin

active: daemon.o main.o watch.o exception.o event.o core.o mask.o $(COMMONS)
	$(CC) $(FLAGS) -pthread $^ -o $(EXE)

client: daemon.o watch.o exception.o event.o core.o mask.o client.o $(COMMONS)
	$(CC) $(FLAGS) -pthread $^ -o client

client.o: ANotifyClient.cpp ANotifyDaemon.h
	$(CC) $(FLAGS) -c ANotifyClient.cpp -o client.o

example: example.o watch.o exception.o event.o core.o mask.o
	$(CC) $(FLAGS) -pthread $^ -o example

example.o: ANotifyExample.cpp
	$(CC) $(FLAGS) -c ANotifyExample.cpp -o example.o

main.o: Main.cpp
	$(CC) $(FLAGS) -c Main.cpp -o main.o

daemon.o: ANotifyDaemon.h ANotifyDaemon.cpp ../common/XMLGeneration.h 
	$(CC) $(FLAGS) -c ANotifyDaemon.cpp -o daemon.o

watch.o: ANotifyWatch.h ANotifyWatch.cpp
	$(CC) $(FLAGS) -c ANotifyWatch.cpp -o watch.o

exception.o: ANotifyException.h ANotifyException.cpp
	$(CC) $(FLAGS) -c ANotifyException.cpp -o exception.o

event.o: ANotifyEvent.h ANotifyEvent.cpp
	$(CC) $(FLAGS) -c ANotifyEvent.cpp -o event.o

mask.o: ANotifyMask.h ANotifyMask.cpp
	$(CC) $(FLAGS) -c ANotifyMask.cpp -o mask.o

core.o: ANotify.h ANotify.cpp ANotifyUtils.h
#	$(CC) $(FLAGS) -c ANotify.cpp -D__APPLE__ -o core.o
	$(CC) $(FLAGS) -c ANotify.cpp -o core.o

xml:
	cd $(CMM)/ && make
#../common/XMLGeneration.h ../common/XMLGeneration.cpp
#	$(CC) $(FLAGS) -c ../common/XMLGeneration.cpp -o xml.o

clean:
	rm -rf *.o *~ *# $(GOALS)