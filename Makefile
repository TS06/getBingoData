CC=g++
CFLG=`pkg-config --cflags opencv`
LFLG=`pkg-config --libs opencv`
SRC=CachNumber.cpp

CatchNumber:CatchNumber.cpp
	${CC} -o CatchNumber CatchNumber.cpp ${CFLG} ${LFLG}
