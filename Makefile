CC=gcc
CFLAGS=-lX11 -Isrc
DEPS=src/wm.h src/util.h
OBJ=src/wm.o src/main.o src/util.o src/wm_setup.o

%o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

wm: $(OBJ)
	gcc -o bin/wm $^ $(CFLAGS)
