CC:=gcc
CFLAGS=-lm

test:
	$(CC) $(CFLAGS) test.c -o test

clean:
	rm test

install: 
	cp matrix.h /usr/include
