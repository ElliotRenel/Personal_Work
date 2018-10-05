#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "game.h"
#include "game_io.h"

#define clear() printf("\033[H\033[J")

 char leafs[4]={'^','>','v','<'};
 char segments[4] = {'|','-','|','-'};
 char corners[4][5] = {"└","┌","┐","┘"};
 char tees[4][5] = {"┴","├","┬","┤"};


game default_game(void){
  piece pe[25]={LEAF,TEE,LEAF,LEAF,LEAF,LEAF,TEE,TEE,CORNER,SEGMENT,LEAF,LEAF,TEE,LEAF,SEGMENT,TEE,TEE,TEE,TEE,TEE,CORNER,LEAF,LEAF,CORNER,LEAF};
  direction dir[25]={E,W,S,E,S,S,S,N,W,N,E,N,W,W,W,S,W,N,E,E,W,N,W,N,S};
  game g = (game)new_game(pe, dir);
  return g;
}


void afficher_game(cgame g){
  int H = game_height(g);
  int W = game_width(g);
  printf("\n\n\t   0 1 2 3 4\n");
  for(int y=H-1; y>=0; y--){
    printf("|\t%d ",y);
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
    printf(" \t|\n");
  }
  printf("\n\n");
}


int main(void){
  game g = default_game();
  clear();
  while(!is_game_over ((cgame) g)){
    afficher_game((cgame)g);
    int x, y;
    printf("Coordonees de la piece a tourner (format <x> <y>): ");
    scanf("%d %d",&x,&y);
    if(!(x<0 || y<0 || x>=game_width((cgame)g) || y>=game_height((cgame)g))){
      rotate_piece_one(g, x, y);
    }else{
      clear();
      printf("\n\n\nPosition invalide, essaie encore\n");
      sleep(1);
      for (int i = 4; i>0; i--){
        printf(".\n");
        sleep(1);
      }
    }
    clear();
  }
  afficher_game((cgame)g);

  printf("\n\nBravo : tu as réussi !\n\n");;
  sleep(5);
  clear();

  return EXIT_SUCCESS;
}
