CC=gcc
CFLAGS=-std=c99 -O3 -Wall -I/usr/include/SDL2
LDFLAGS=-rdynamic -lm -lSDL2 -lSDL2_ttf -lSDL2_image

ALL: game_sdl

game_sdl: net_sdl.o game_sdl.o 

clean:
	rm -f *~ *.o game_sdl
