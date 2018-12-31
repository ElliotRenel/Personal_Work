#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "game.h"
#include "game_io.h"

 

//comentaire...

game default_game(cgame* soluce){
  piece pe[25]={LEAF,TEE,LEAF,LEAF,LEAF,LEAF,TEE,TEE,CORNER,SEGMENT,LEAF,LEAF,TEE,LEAF,SEGMENT,TEE,TEE,TEE,TEE,TEE,CORNER,LEAF,LEAF,CORNER,LEAF};
  direction dir[25]={E,W,S,E,S,S,S,N,W,S,E,N,W,W,W,S,W,N,E,E,W,N,W,N,S};
  game g = (game)new_game(pe, dir);
  return g;
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

int main(void){
  cgame sol = new_game_empty();
  game g = default_game(&sol);
  while(!is_game_over ((cgame) g)){
    afficher_game((cgame)g);
    int x, y;
    printf("Coordonees de la piece a tourner (format <x> <y>): ");
    scanf("%d %d",&x,&y);
    if(!(x<0 || y<0 || x>=game_width((cgame)g) || y>=game_height((cgame)g))){
      rotate_piece_one(g, x, y);
    }
  }
  afficher_game((cgame)g);
  delete_game(g);
  return EXIT_SUCCESS;
}
