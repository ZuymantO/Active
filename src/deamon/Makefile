
CC=gcc
BIN=bin
INC=includes
LIB=lib
OBJ=obj
SRC=src
GLIB=ar
FLAGS=-Wall -ansi -g
all: deamon

${OBJ}/ANotify.o: ${SRC}/ANotify/ANotify.cpp
	${CC} -o $@ -c $< -I${INC} ${FLAGS}
	
${OBJ}/ANotifyMask.o: ${SRC}/ANotify/ANotifyMask.cpp
	${CC} -o $@ -c $< -I${INC} ${FLAGS}

${OBJ}/ANotifyEvent.o: ${SRC}/ANotify/ANotifyEvent.cp
	${CC} -o $@ -c $< -I${INC} ${FLAGS}
	
${OBJ}/ANotifyException.o:${SRC}/ANotify/ANotifyException.cpp
	${CC} -o $@ -c $< -I${INC} ${FLAGS}
	
${OBJ}/ANotifyWatch.o: ${SRC}/ANotify/ANotifyMask.cpp
	${CC} -o $@ -c $< -I${INC} ${FLAGS}

${OBJ}/ANotifyUtil.o: ${SRC}/ANotify/ANotifyUtils.hpp
	${CC} -o $@ -c $< -I${INC} ${FLAGS}





deamon: ${OBJ}/ANotifyUtil.o ${OBJ}/ANotifyMask.o ${OBJ}/ANotifyException.o ${OBJ}/ANotifyEvent.o ${OBJ}/ANotifyWatch.o ${OBJ}/ANotify.o 
	${CC} -o $@ $^ -I${INC} ${FLAGS}

exec: ${BIN}/deamon cpfile

cpfile: 
	cp ${BIN}/deamon ../../${BIN}

clean: 
	rm -rf ./*~ ${SRC}/*~ ${OBJ}/* ${BIN}/*





