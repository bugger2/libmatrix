CC:=gcc
CFLAGS=-lm

test: test.o
	$(CC) test.o -o test $(CFLAGS)

test.o: test.c
	$(CC) test.c -c $(CFLAGS)

clean:
	rm test

install: 
	cp matrix.h /usr/include
