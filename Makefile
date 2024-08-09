CC:=gcc
CFLAGS=-lm

test:
	$(CC) test.c -o test $(CFLAGS)

clean:
	rm test

install: 
	cp matrix.h /usr/include
