#CC=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++
CC=g++
#FLAGS=-Wall
FLAGS=
OBJ=active


all: active

active: main.o watch.o exception.o event.o core.o mask.o
	$(CC) $(FLAGS) -pthread $^ -o $(OBJ)

example: example.o watch.o exception.o event.o core.o mask.o
	$(CC) $(FLAGS) -pthread $^ -o example

example.o: ANotifyExample.cpp
	$(CC) $(FLAGS) -c ANotifyExample.cpp -o example.o

main.o: Main.cpp
	$(CC) $(FLAGS) -c Main.cpp -o main.o

watch.o: ANotifyWatch.h ANotifyWatch.cpp
	$(CC) $(FLAGS) -c ANotifyWatch.cpp -o watch.o

exception.o: ANotifyException.h ANotifyException.cpp
	$(CC) $(FLAGS) -c ANotifyException.cpp -o exception.o

event.o: ANotifyEvent.h ANotifyEvent.cpp
	$(CC) $(FLAGS) -c ANotifyEvent.cpp -o event.o

mask.o: ANotifyMask.h ANotifyMask.cpp
	$(CC) $(FLAGS) -c ANotifyMask.cpp -o mask.o

core.o: ANotify.h ANotify.cpp
#	$(CC) $(FLAGS) -c ANotify.cpp -D__APPLE__ -o core.o
	$(CC) $(FLAGS) -c ANotify.cpp -o core.o

clean:
	rm -rf *.o *~ *# $(OBJ) example
