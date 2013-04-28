CC=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++
#FLAGS=-Wall
FLAGS=
OBJ=active


all: active

#active: main.o anotify.o
active: main.o watch.o exception.o event.o core.o
#	$(CC) -lpthread main.o anotify.o -o $(OBJ)
	$(CC) $(FLAGS) -pthread main.o watch.o exception.o event.o core.o -o $(OBJ)

main.o: 
	$(CC) $(FLAGS) -c Main.cpp -o main.o

#anotify.o: watch.o exception.o event.o core.o
#	$(CC) $(FLAGS) watch.o exception.o event.o core.o -o anotify.o

watch.o: ANotifyWatch.h
	$(CC) $(FLAGS) -c ANotifyWatch.cpp -o watch.o

exception.o: ANotifyException.h
	$(CC) $(FLAGS) -c ANotifyException.cpp -o exception.o

event.o: ANotifyEvent.h
	$(CC) $(FLAGS) -c ANotifyEvent.cpp -o event.o

core.o: ANotify.h
	$(CC) $(FLAGS) -c ANotify.cpp -D__APPLE__ -o core.o

clean:
	rm -rf *.o *~ *# $(OBJ)
