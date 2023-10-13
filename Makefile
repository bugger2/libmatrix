CC:=clang
CFLAGS=-lm

all: libmatrix

libmatrix:
	$(CC) $(CFLAGS) -shared -o libmatrix.so -fPIC matrix.c

clean:
	rm libmatrix.so

install: libmatrix
	if [ -f /usr/bin/doas ]; then
		doas cp matrix.h /usr/include
		doas cp libmatrix.so /usr/lib
	else
		sudo cp libmatrix.so /usr/lib
		sudo cp matrix.h /usr/include
	fi

remake: clean
	make
