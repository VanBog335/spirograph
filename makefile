CC=gcc
TCC=/c/tcc/tcc
CFLAGS=-L./ -lraylib -s -O2 -Wall
TCFLAGS=-L./ -lraylib

all:
	#$(TCC) $(TCFLAGS) -o clgr.exe clgr.c 
	#$(TCC) $(TCFLAGS) -o circles.exe circles.c 
	#$(TCC) $(TCFLAGS) -o rect.exe rect.c
	$(CC) $(CFLAGS) -o waything.exe waything.c
	#$(TCC) $(TCFLAGS) -o launc.exe launcher.c
	./waything