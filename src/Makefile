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

main.o: 
	$(CC) $(FLAGS) -c Main.cpp -o main.o

example.o:
	$(CC) $(FLAGS) ${DIR_PATH} ${FILE_PATH} -c ANotifyExample.cpp -o example.o

#anotify.o: watch.o exception.o event.o core.o
#	$(CC) $(FLAGS) watch.o exception.o event.o core.o -o anotify.o

watch.o: ANotifyWatch.h
	$(CC) $(FLAGS) -c ANotifyWatch.cpp -o watch.o

exception.o: ANotifyException.h
	$(CC) $(FLAGS) -c ANotifyException.cpp -o exception.o

event.o: ANotifyEvent.h
	$(CC) $(FLAGS) -c ANotifyEvent.cpp -o event.o

core.o: ANotify.h
	$(CC) $(FLAGS) -c ANotify.cpp -o core.o

mask.o: ANotifyMask.h
	$(CC) $(FLAGS) -c ANotifyMask.cpp -o mask.o

clean:
	rm -rf *.o *~ *# $(OBJ) $(EXAMPLE)
