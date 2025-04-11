CC=gcc
TCC=/c/tcc/tcc
CFLAGS=-L./ -lraylib -s -O2 -Wall
TCFLAGS=-L./ -lraylib

all:
	$(CC) $(CFLAGS) -o waything.exe waything.c
	./waything
