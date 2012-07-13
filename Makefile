CC=gcc
CFLAGS=-std=c99 -ggdb
LIBS=
all: lz77
clean:
	rm lz77 *.o
lz77: lz77.o window.o
	${CC} ${CFLAGS} ${LIBS} -o lz77 lz77.o window.o
lz77.o: lz77.c
	${CC} ${CFLAGS} -c lz77.c
window.o: window.h
	${CC} ${CFLAGS} -c window.c