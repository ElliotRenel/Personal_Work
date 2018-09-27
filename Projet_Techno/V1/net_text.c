#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_io.h"

typedef unsigned long uint;

int main(void){
  piece pe[25]={LEAF,LEAF,LEAF,TEE,CORNER,SEGMENT,CORNER,TEE,CORNER,LEAF,TEE,TEE,SEGMENT,LEAF,LEAF,LEAF,TEE,SEGMENT,SEGMENT,CORNER,LEAF,TEE,SEGMENT,SEGMENT,LEAF};
  direction dir[25]={N,N,E,N,W,N,E,N,S,S,E,N,E,W,N,S,E,E,E,S,E,S,W,W,W};
  cgame soluce = new_game(pe, dir);
  game g = (game)soluce;
  shuffle_dir(g);
  return EXIT_SUCCESS;
}
