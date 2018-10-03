#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_io.h"


 char leafs[4][5]={"^",">","v","<"};
 char segments[4][5] = {"|","-","|","-"};
 char corners[4][5] = {"└","┌","┐","┘"};
 char tees[4][5] = {"┴","├","┬","┤"};

char** PIECES[4] ={*leafs, segments, corners, tees};



game default_game(cgame* soluce){
  piece pe[25]={LEAF,LEAF,LEAF,TEE,CORNER,SEGMENT,CORNER,TEE,CORNER,LEAF,TEE,TEE,SEGMENT,LEAF,LEAF,LEAF,TEE,SEGMENT,SEGMENT,CORNER,LEAF,TEE,SEGMENT,SEGMENT,LEAF};
  direction dir[25]={N,N,E,N,W,N,E,N,S,S,E,N,E,W,N,S,E,E,E,S,E,S,W,W,W};
  *(soluce) = new_game(pe, dir);
  game g = (game)(*soluce);
  shuffle_dir(g);
  return g;
}


void afficher_game(cgame g){
  int H = game_height(g);
  int W = game_width(g);
  for(int y=H-1; y>=0; y--){
    for(int x=0; x>=W-1; x++){
      piece p = get_piece(g, x, y);
      direction d = get_current_dir(g, x, y);
      printf("%s",PIECES[p][d]);
    }
    printf("\n");
  }
}


int main(void){
  cgame sol = new_game_empty();
  game g = default_game(&sol);
  afficher_game((cgame)g);

  return EXIT_SUCCESS;
}
