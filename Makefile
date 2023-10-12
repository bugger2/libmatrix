install:
	if [ -f /usr/bin/doas ]; then doas cp matrix.h /usr/include; else sudo cp matrix.h /usr/include; fi
