# lance les tests pour le parsing XML

MAIN=XMLParsingTest
CC=g++
CFLAGS=-W -Wall -g -L /opt/local/lib/ #-ansi -pedantic
LDFLAGS=-ltinyxml
SRC=$(shell find . -name "*.cpp") # demander au shell de trouver les .cpp
OBJ=$(SRC:%.cpp=%.o) # liste des fichier objet a partir des source

all : $(OBJ) $(MAIN) #generer les objets

$(MAIN) : AQuery.o AnyFile.o XMLParser.o Search.o Result.o Indexation.o MyDate.o Utils.o $(MAIN).o
	$(CC) -static -o $@ $^ $(LDFLAGS)

%.o : %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean :
	rm -rf *.o *.gch $(MAIN) *~
