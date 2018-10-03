#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_io.h"

/*
char leafs[4]={'^','>','v','<'};
char tees[4]={'┴','├','┬','┤'};
char corners[4]={'└','','┐','┘'};
char segments[4]={'|','-','|','-'};
*/

game default_game(cgame* soluce){
  piece pe[25]={LEAF,LEAF,LEAF,TEE,CORNER,SEGMENT,CORNER,TEE,CORNER,LEAF,TEE,TEE,SEGMENT,LEAF,LEAF,LEAF,TEE,SEGMENT,SEGMENT,CORNER,LEAF,TEE,SEGMENT,SEGMENT,LEAF};
  direction dir[25]={N,N,E,N,W,N,E,N,S,S,E,N,E,W,N,S,E,E,E,S,E,S,W,W,W};
  *(soluce) = new_game(pe, dir);
  game g = (game)(*soluce);
  shuffle_dir(g);
  return g;
}

/*
void afficher_game(game g){
  int H = game_height((cgame)g);
  int W = game_width((cgame)g);
  int i;
  char ligne[W];
  for (i=H*W-1; i>0; i--){
    if(i%W==W-1){
      printf("%s\n",ligne);
    }
    ligne[i%W];
  }
}
*/

int main(void){

  cgame sol = new_game_empty();
  game g = default_game(&sol);
  printf("%u",sizeof(g));

  return EXIT_SUCCESS;
}
