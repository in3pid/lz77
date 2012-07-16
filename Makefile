CC=gcc
OBJS=window.o match.o lz77.o
CFLAGS=-std=c99 -ggdb #-pg
LIBS=
all: lz77
test: test.c window.o
	${CC} ${CFLAGS} ${LIBS} -o test test.c window.o
clean:
	rm -f lz77 *.o
lz77: ${OBJS}
	${CC} ${CFLAGS} ${LIBS} -o lz77 ${OBJS}
%.o: %.c
	${CC} ${CFLAGS} -o $@ -c $<