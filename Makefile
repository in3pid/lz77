CC=gcc
CFLAGS=-std=c99 -ggdb
LIBS=
all: lz77
clean:
	rm lz77 *.o
lz77: lz77.c lz77.h window.h
	${CC} ${CFLAGS} ${LIBS} -o lz77 lz77.c