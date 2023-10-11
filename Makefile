CC:=clang
CFLAGS=-lm

all: libmatrix

libmatrix:
	$(CC) $(CFLAGS) -shared -o libmatrix.so -fPIC matrix.c

clean:
	rm libmatrix.so

remake: clean
	make
