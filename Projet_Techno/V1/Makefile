CC = gcc
CFLAGS = -Wall -std=c99
<<<<<<< HEAD
LDFLAGS = -lgame
=======
LDFLAGS = -lgame -L.
>>>>>>> char_not_string
OBJECTS = game.o game_io.o

all : net_text

net_text : net_text.o libgame.a
<<<<<<< HEAD
	$(CC) net_text.o $(LDFLAGS) -o $@

net_text.o : net_text.c game.h game_io.h
	$(CC) $(CFLAGS) -c $<
=======
	$(CC) $< $(LDFLAGS) -o $@

net_text.o : net_text.c game.h game_io.h
>>>>>>> char_not_string

libgame.a : $(OBJECTS)
	ar rcs $@ $(OBJECTS)

.PHONY: clean

clean :
<<<<<<< HEAD
	rm *.a net_text net_text.o
=======
	rm -f *.a net_text net_text.o
>>>>>>> char_not_string
