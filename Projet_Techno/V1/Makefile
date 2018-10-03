CC = gcc
CFLAGS = -Wall -std=c99
OBJECTS = game.o game_io.o

all : net_text

net_text : net_text.c libgame.a
	$(CC) $(CFLAGS) -o $@ net_text.c libgame.a

libgame.a : $(OBJECTS)
	ar rcs $@ $(OBJECTS)

.PHONY: clean

clean :
	rm *.a net_text
