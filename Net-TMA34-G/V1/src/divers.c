#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "game.h"
#include "game_io.h"

void pb(bool* t){
  for(int i=0; i<25; i++){
    printf("%d : %d\n",i,t[i]?1:0);
  }
}

void afficher_game(cgame g){
  char leafs[4]={'^','>','v','<'};
  char segments[4] = {'|','-','|','-'};
  char corners[4][5] = {"└","┌","┐","┘"};
  char tees[4][5] = {"┴","├","┬","┤"};
  int H = game_height(g);
  int W = game_width(g);
  printf("\n\n");
  for(int y=H-1; y>=0; y--){
    printf("| ");
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
    printf("  |\n");
  }
  printf("\n _ _ _ _ _ _ _\n\n");
}

int main(int argc, char** argv){
  piece pcc[25] = {LEAF,SEGMENT,SEGMENT,SEGMENT,LEAF,LEAF,SEGMENT,SEGMENT,SEGMENT,LEAF,LEAF,SEGMENT,SEGMENT,SEGMENT,LEAF,LEAF,SEGMENT,SEGMENT,SEGMENT,LEAF,LEAF,SEGMENT,SEGMENT,SEGMENT,LEAF};
  direction dcc[25] =  {E,E,E,E,W,E,E,E,E,W,E,E,E,E,W,E,E,E,E,W,E,E,E,E,W};
  cgame g = (cgame) new_game(pcc,dcc);
  afficher_game(g);
  return EXIT_SUCCESS;
}
