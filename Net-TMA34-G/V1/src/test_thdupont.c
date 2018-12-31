#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "game.h"
#include "tools.h"

// Définition variables globale
piece P[25] = {LEAF,CORNER,LEAF,CORNER,CORNER,CORNER,TEE,TEE,TEE,SEGMENT,LEAF,SEGMENT,TEE,LEAF,LEAF,LEAF,SEGMENT,TEE,CORNER,LEAF,LEAF,SEGMENT,TEE,TEE,LEAF};
direction D[25] = {N,N,W,N,W,E,S,N,W,N,E,W,W,S,S,E,E,W,N,W,E,E,S,S,W};

/* ********** TEST EMPTY ********** */

int test_empty(int argc, char *argv[])
{
    game g = new_game_empty();
    assert(g);
    int w = game_width(g);
    int h = game_height(g);
    if(w!=DEFAULT_SIZE || h!=DEFAULT_SIZE){
        fprintf(stderr, "Error: new_game_empty asigned wrong width or height!\n");
        return EXIT_FAILURE;
    }
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
        {
            if (get_piece(g, x, y) != EMPTY)
            {
                fprintf(stderr, "Error: piece (%d,%d) is not empty!\n", x, y);
                return EXIT_FAILURE;
            }
        }
    delete_game(g);
    g = NULL;
    return EXIT_SUCCESS;
}

/* ********** USAGE ********** */

#ifndef TESTFW

void usage(int argc, char *argv[])
{
    fprintf(stderr, "Usage: %s <testname>\n", argv[0]);
    exit(EXIT_FAILURE);
}

#endif

/* ***** TEST opposite_dir ***** */

int test_opposite_dir(int argc, char *argv[]){      /* Appel avec opposite_dir */
    direction test_tab[4] = {N,E,S,W};
    direction opposite_dir_tab[4] = {S,W,N,E};
    for(int i=0; i<4; i++){
        if(opposite_dir(test_tab[i]) != opposite_dir_tab[i]){
          fprintf(stderr, "Error: Opposite direction of %c is not %c!", test_tab[i], opposite_dir_tab[i]);
          return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;

}

/* ******** TEST copy_game ********* */

int test_copy_game(int argc, char *argv[]){
  piece pe[25]={LEAF,LEAF,LEAF,TEE,CORNER,SEGMENT,CORNER,TEE,CORNER,LEAF,TEE,TEE,SEGMENT,LEAF,LEAF,LEAF,TEE,SEGMENT,SEGMENT,CORNER,LEAF,TEE,SEGMENT,SEGMENT,LEAF};
  direction dir[25]={N,N,E,N,W,N,E,N,S,S,E,N,E,W,N,S,E,E,E,S,E,S,W,W,W};
  game g = new_game(pe, dir);
  game g_copy = copy_game(g);
  int g_height = game_height(g);
  int g_width = game_width(g);
  if(!(game_height(g_copy)== g_height && game_width(g_copy)== g_width)){
      fprintf(stderr, "Error: Height and/or width of the copy is not equal to original!");
      return EXIT_FAILURE;
  }
  for(int i =0; i < g_width; i++){
    for(int j = 0; j < g_height; j++){
      if(!((get_piece(g, i, j)==get_piece(g_copy, i, j)) && (get_current_dir(g, i, j)==get_current_dir(g_copy, i, j)))){
        fprintf(stderr, "Error: Piece or direction is not the same! Coordonates: %d %d", i, j);
        return EXIT_FAILURE;
      }
    }
  }
  delete_game(g);
  delete_game(g_copy);
  return EXIT_SUCCESS;
}

/* ******** TEST game_height ***** */

int test_game_height(int argc, char *argv[]){
    game g = new_game_empty();
    //printf("Height of empty: Is it equal to DEFAULT_SIZE? %s\n", game_height(g)==DEFAULT_SIZE ? " Yes!\n" : " No!\n");
    piece pe[25]={LEAF,LEAF,LEAF,TEE,CORNER,SEGMENT,CORNER,TEE,CORNER,LEAF,TEE,TEE,SEGMENT,LEAF,LEAF,LEAF,TEE,SEGMENT,SEGMENT,CORNER,LEAF,TEE,SEGMENT,SEGMENT,LEAF};
    direction dir[25]={N,N,E,N,W,N,E,N,S,S,E,N,E,W,N,S,E,E,E,S,E,S,W,W,W};
    game g2 = new_game(pe, dir);
    //printf("Height of g2: Is it equal to DEFAULT_SIZE?%s\n", game_height(g2)==DEFAULT_SIZE ? " Yes!\n" : " No!\n");
    if(game_height(g2)!=DEFAULT_SIZE || game_height(g)!=DEFAULT_SIZE){
      fprintf(stderr, "Height not equal to DEFAULT_SIZE!");
      return EXIT_FAILURE;
    }
    delete_game(g);
    delete_game(g2);
    return EXIT_SUCCESS;
}


/* ****** TEST delete_game ********* */

int test_delete_game(int argc, char *argv[]){
    game g = new_game_empty();
    delete_game(g);
    return EXIT_SUCCESS;
}

/* *********** TEST get_current_dir ********** */

int test_get_current_dir(int argc, char *argv[]){
    game g = new_game_empty();
    for(int i =0; i<game_height(g); i++){
      for(int j=0; j<game_width(g); j++){
        if(get_current_dir(g, j, i)!= N){
          return EXIT_FAILURE;
        }
      }
    }
    game g2 = new_game(P,D);
    int n = 0;
    for(int i =0; i<game_height(g2); i++){
      for(int j=0; j<game_width(g2); j++){
        if(get_current_dir(g2, j, i)!= D[n]){
          return EXIT_FAILURE;
        }
        n++;
      }
    }
    delete_game(g);
    delete_game(g2);
    return EXIT_SUCCESS;
}



/* ********** MAIN ROUTINE ********** */

#ifndef TESTFW

int main(int argc, char *argv[])
{
    if (argc == 1)
        usage(argc, argv);

    printf("=> RUN TEST \"%s\"\n", argv[1]);

    int status;
    if (strcmp("empty", argv[1]) == 0)
        status = test_empty(argc, argv);
    else if (strcmp("opposite_dir", argv[1]) == 0)
        status = test_opposite_dir(argc,argv);
    else if (strcmp("copy_game", argv[1]) == 0)
        status = test_copy_game(argc,argv);
    else if (strcmp("game_height", argv[1]) == 0)
        status = test_game_height(argc,argv);
    else if (strcmp("delete_game", argv[1]) == 0)
        status = test_delete_game(argc,argv);
    else if (strcmp("get_current_dir", argv[1]) == 0)
        status = test_get_current_dir(argc,argv);
    else
    {
        fprintf(stderr, "Error: test %s not found!\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (status != EXIT_SUCCESS)
        printf("FAILURE (status %d)\n", status);
    else
        printf("SUCCESS (status %d)\n", status);

    return EXIT_SUCCESS;
}

#endif
