#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_io.h"


char leafs[4]={'^','>','v','<'};
char segments[4]={'|','-','|','-'};
char corners[4]={(char)192,(char)218,(char)191,(char)217};
char tees[4]={(char)193,(char)195,(char)194,(char)180};

char* PIECES[4] ={leafs, segments, corners, tees};



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
  char str[4]="    ";
  for(int y=H-1; y>=0; y--){
    for(int x=W-1; x>=0; x--){
      piece p = get_piece(g, x, y);
      direction d = get_current_dir(g, x, y);
      str[x]=PIECES[p][d];
    }
    printf("%s\n",str);
  }
}


int main(void){

  cgame sol = new_game_empty();
  game g = default_game(&sol);
  afficher_game((cgame)g);

  return EXIT_SUCCESS;
}
