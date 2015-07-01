CC=gcc
CFLAGS=-lX11 -Isrc
DEPS=src/wm.h
OBJ=src/wm.o src/main.o

%o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

wm: $(OBJ)
	gcc -o bin/wm $^ $(CFLAGS)
