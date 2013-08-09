CC=gcc -std=c99 -ggdb

hr: Makefile hr.c
	$(CC) hr.c -o hr
