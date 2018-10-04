CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lgame -L.
OBJECTS = game.o game_io.o

all : net_text

net_text : net_text.o libgame.a
	$(CC) $< $(LDFLAGS) -o $@

net_text.o : net_text.c game.h game_io.h

libgame.a : $(OBJECTS)
	ar rcs $@ $(OBJECTS)

.PHONY: clean

clean :
	rm -f *.a net_text net_text.o
