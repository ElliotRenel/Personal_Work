#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_io.h"


 char leafs[4]={'^','>','v','<'};
 char segments[4] = {'|','-','|','-'};
 char corners[4][5] = {"└","┌","┐","┘"};
 char tees[4][5] = {"┴","├","┬","┤"};





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
    for(int x=0; x<W; x++){
      piece p = get_piece(g, x, y);
      direction d = get_current_dir(g, x, y);
      if(p==LEAF){
        printf(" %c",leafs[d]);
      }else if(p==SEGMENT){
        printf(" %c",segments[d]);
      }else if(p==CORNER){
        printf(" %s",corners[d]);
      }else if(p==TEE){
        printf(" %s",tees[d]);
      }
    }
    printf("\n");
  }
}


int main(void){
  cgame sol = new_game_empty();
  game g = default_game(&sol);

  while(!is_game_over ((cgame) g)){
    afficher_game((cgame)g);
    int x, y;
    printf("Coordonees de la piece a tourner (format <x> <y>): ");
    scanf("%d %d",&x,&y);
    
  }

  return EXIT_SUCCESS;
}
