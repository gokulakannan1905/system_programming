#macro section
COM = g++
BIN = bin/
OBJ = obj/
INC = include/
SRC = src/
CFLAG = -c
OFLAG = -o

#target section
all: build-server build-client

#build-server target to build server binary 
build-server: compile-func 
	${COM} ${OFLAG} ${BIN}server15 ${SRC}server.cpp ${OBJ}func.o
compile-func:
	${COM} ${CFLAG} ${OFLAG} ${OBJ}func.o ${SRC}func.cpp

#build-client target to build client binary
build-client: 
	${COM} ${OFLAG} ${BIN}client15 ${SRC}client.cpp

#targets to run the program
ss:
	./${BIN}server15
sc:
	./${BIN}client15

#target to clean the bin/ and obj/ directory
clean:
	rm ${OBJ}func.o ${BIN}server15 ${BIN}client15

#end of makefile
