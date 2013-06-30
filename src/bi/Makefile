CC=g++
FLAGS=-Wall
LIBS=-pthread
EXE=bi

all: $(EXE)

$(EXE): bi.o base_com.o indexation_com.o search_com.o exception.o main.o
	$(CC) $(FLAGS) $(LIBS) $^ -o $(EXE)

bi.o: IndexationBase.h IndexationBase.cpp IndexationCom.h BaseCom.h SearchCom.h
	$(CC) $(FLAGS) -c IndexationBase.cpp -o bi.o

base_com.o: BaseCom.h BaseCom.cpp BaseException.h
	$(CC) $(FLAGS) -c BaseCom.cpp -o base_com.o

search_com.o: SearchCom.h SearchCom.cpp BaseException.h
	$(CC) $(FLAGS) -c SearchCom.cpp -o search_com.o

indexation_com.o: IndexationCom.h IndexationCom.cpp BaseException.h
	$(CC) $(FLAGS) -c IndexationCom.cpp -o indexation_com.o

exception.o: BaseException.h BaseException.cpp
	$(CC) $(FLAGS) -c BaseException.cpp -o exception.o

main.o: Main.cpp IndexationBase.h BaseException.h
	$(CC) $(FLAGS) -c Main.cpp -o main.o

clean:
	rm -rf $(EXE) *.o *# *~