FLAGS=-Wall
EXE=bi

all: bi

bi: search_com.o indexation_com.o exception.o bi.o main.o
	g++ $(FLAGS) -pthread $^ -o bi

bi.o: IndexationBase.h IndexationBase.cpp BaseException.h
	g++ -c IndexationBase.cpp -o bi.o

search_com.o: SearchCom.h SearchCom.cpp BaseException.h
	g++ -c SearchCom.cpp -o search_com.o

indexation_com.o: IndexationCom.h IndexationCom.cpp BaseException.h
	g++ -c IndexationCom.cpp -o indexation_com.o

exception.o: BaseException.h BaseException.cpp
	g++ -c BaseException.cpp -o exception.o

main.o: BaseException.h IndexationBase.h Main.cpp
	g++ -c Main.cpp -o main.o

clean:
	rm -rf *.o $(EXE) *~ *#