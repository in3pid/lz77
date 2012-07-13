CC=gcc
CFLAGS=-std=c99 -ggdb
LIBS=
all: lz77
clean:
	rm -f lz77 *.o
lz77: lz77.o window.o
	${CC} ${CFLAGS} ${LIBS} -o lz77 lz77.o window.o
%.o: %.c
	${CC} ${CFLAGS} -o "$@" -c "$<"
