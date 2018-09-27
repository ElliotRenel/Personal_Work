CC = gcc
CFLAGS = -Wall -std=c99
OBJECTS = $(wildcard *.o)

all : libgame net_text

libgame : $(OBJECTS)
	ar rcs libgame.a $(OBJECTS)

net_text : net_text.c libgame.a
	$(CC) -o $@ net_text.c libgame.a

clean :
	del *.a *.exe
