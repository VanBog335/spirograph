CC=gcc
TCC=/c/tcc/tcc
CFLAGS=-L./ -lraylib -s -O2 -Wall
TCFLAGS=-L./ -lraylib -Wall

all:
	@echo "Building..."
	@$(CC) $(CFLAGS) -o waything.exe waything.c
	@echo "Complete!"
	
	@#$(CC) $(CFLAGS) -o waything-old.exe waything-old.c

git:
	git commit -am "." ; git push