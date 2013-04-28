CC=g++
#FLAGS=-Wall
FLAGS=
OBJ=active
EXAMPLE=example


#ifneq (${DIR_PATH},)
#	DIR_VALUE='-D DIR_PATH=\"${DIR_PATH}\"'
#else 
#endif
#ifneq (${FILE_PATH},)
#	FILE_VALUE='-D FILE_PATH=\"${FILE_PATH}\"'
#else
#endif

all: active

example: example.o core.o watch.o event.o exception.o mask.o
	$(CC) $(FLAGS) -pthread example.o watch.o exception.o core.o mask.o -o example

#active: main.o anotify.o
active: main.o watch.o exception.o event.o core.o mask.o
#	$(CC) -lpthread main.o anotify.o -o $(OBJ)
	$(CC) $(FLAGS) -pthread main.o watch.o exception.o event.o core.o mask.o -o $(OBJ)

main.o: Main.cpp
	$(CC) $(FLAGS) -c Main.cpp -o main.o

example.o: ANotifyExample.cpp
	$(CC) $(FLAGS) ${DIR_PATH} ${FILE_PATH} -c ANotifyExample.cpp -o example.o

#anotify.o: watch.o exception.o event.o core.o
#	$(CC) $(FLAGS) watch.o exception.o event.o core.o -o anotify.o

watch.o: ANotifyWatch.h ANotifyWatch.cpp
	$(CC) $(FLAGS) -c ANotifyWatch.cpp -o watch.o

exception.o: ANotifyException.h ANotifyException.cpp
	$(CC) $(FLAGS) -c ANotifyException.cpp -o exception.o

event.o: ANotifyEvent.h ANotifyEvent.cpp
	$(CC) $(FLAGS) -c ANotifyEvent.cpp -o event.o

core.o: ANotify.h ANotify.cpp
	$(CC) $(FLAGS) -c ANotify.cpp -o core.o

mask.o: ANotifyMask.h ANotifyMask.cpp
	$(CC) $(FLAGS) -c ANotifyMask.cpp -o mask.o

clean:
	rm -rf *.o *~ *# $(OBJ) $(EXAMPLE)
