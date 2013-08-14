CC=gcc
CFLAGS=-std=c99 -O3 -D_XOPEN_SOURCE

hr: Makefile hr.c
	$(CC) $(CFLAGS) hr.c -o hr

clean:
	rm -f hr
