CC=gcc
CFLAGS=-Wall -std=c11
LDFLAGS=-lX11
DEPS=src/wm.h src/util.h
OBJ=src/wm.o src/main.o src/util.o src/wm_setup.o

CFLAGS += $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)

%o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

wm: $(OBJ)
	gcc -o bin/owlwm $^ $(CFLAGS) $(LDFLAGS)
