CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lgame
OBJECTS = game.o game_io.o

all : net_text

net_text : net_text.o libgame.a
	$(CC) net_text.o $(LDFLAGS) -o $@

net_text.o : net_text.c game.h game_io.h
	$(CC) $(CFLAGS) -c $<

libgame.a : $(OBJECTS)
	ar rcs $@ $(OBJECTS)

.PHONY: clean

clean :
	rm *.a net_text net_text.o
