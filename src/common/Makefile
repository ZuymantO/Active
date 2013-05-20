# lance les tests pour le parsing XML

MAIN=XMLParsingTest
CC=g++
CFLAGS=-W -Wall -ansi -pedantic
LDFLAGS=

all : $(MAIN)

$(MAIN) : $(MAIN).o XMLParser.o Search.o Result.o Indexation.o MyDate.o Utils.o tinyxml.o tinystr.o tinyxmlerror.o tinyxmlparser.o
	$(CC) -static -o $@ $^ $(LDFLAGS)

%.o : %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean :
	rm -rf *.o $(MAIN) *~
