#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "game.h"
#include "tools.h"

int test_set_piece(int argc, char *argv[])
{
  direction tab_d[NB_DIR] = {N,E,S,W};
  char char_d[] ={'N','E','S','W'};
  piece tab_p[NB_PIECE_TYPE] = {LEAF,SEGMENT,CORNER,TEE};
  game g = new_game_empty();
  assert(g);
  int w = game_width(g);
  int h = game_height(g);
  for (int y = 0; y < h; y++){
    for (int x = 0; x < w;x++){
      for (int i =0;i<NB_DIR;i++){
        for (int j =0;j<NB_PIECE_TYPE;j++){
          set_piece(g,x,y,tab_p[j],tab_d[i]);
          piece piece_test=get_piece(g,x,y);
          if (piece_test!=tab_p[j]){
            fprintf(stderr, "wrong piece at (%d,%d) coordinates\n",x,y);
            return EXIT_FAILURE;
          }
          direction dir_test=get_current_dir(g,x,y);
          if (dir_test!=tab_d[i]){
            fprintf(stderr, "direction %c at (%d,%d) coordinates was suppose to be %c\n",dir_test,x,y,char_d[i]);
            return EXIT_FAILURE;
          }
        }
      }
    }
  }
  delete_game(g);
  g = NULL;
  return EXIT_SUCCESS;
}

int test_game_width(int argc, char *argv[]){
    game g = new_game_empty();
    assert(g);
    if (game_width(g)!=DEFAULT_SIZE){
      fprintf(stderr, "the width of the game was suppose to be %d and not %d\n",DEFAULT_SIZE,game_width(g));
      return EXIT_FAILURE;
    }
    //printf("width of empty:%d\n", game_width(g));
    piece pe[25]={LEAF,LEAF,LEAF,TEE,CORNER,SEGMENT,CORNER,TEE,CORNER,LEAF,TEE,TEE,SEGMENT,LEAF,LEAF,LEAF,TEE,SEGMENT,SEGMENT,CORNER,LEAF,TEE,SEGMENT,SEGMENT,LEAF};
    direction dir[25]={N,N,E,N,W,N,E,N,S,S,E,N,E,W,N,S,E,E,E,S,E,S,W,W,W};
    game g2 = new_game(pe, dir);
    assert(g2);
    if (game_width(g2)!=DEFAULT_SIZE){
      fprintf(stderr, "the width of the game was suppose to be %d and not %d\n",DEFAULT_SIZE,game_width(g2) );
      return EXIT_FAILURE;
    }
    //printf("width of g2:%d (5x5)\n", game_width(g2));
    delete_game(g);
    delete_game(g2);
    return EXIT_SUCCESS;
}


int test_rotate_one(int argc,char *argv[]){
  piece pe[25]={LEAF,LEAF,LEAF,TEE,CORNER,SEGMENT,CORNER,TEE,CORNER,LEAF,TEE,TEE,SEGMENT,LEAF,LEAF,LEAF,TEE,SEGMENT,SEGMENT,CORNER,LEAF,TEE,SEGMENT,SEGMENT,LEAF};
  direction dir[25]={N,N,E,N,W,N,E,N,S,S,E,N,E,W,N,S,E,E,E,S,E,S,W,W,W};
  game g = new_game(pe,dir);
  assert(g);
  for (unsigned int x = 0;x<game_height(g);x++){
    for (unsigned int y = 0;y<game_width(g);y++){
      direction d = get_current_dir(g,x,y);
      if (d==N){
        d=E;
      }else if (d==E){
        d=S;
      }else if (d==S){
        d=W;
      }else if (d==W){
        d=N;
      }
      rotate_piece_one(g,x,y);
      if (get_current_dir(g,x,y)!=d){
        fprintf(stderr, "piece didn't rotate once\n");
        return EXIT_FAILURE;
      }
    }
  }
  delete_game(g);
  return EXIT_SUCCESS;
}

int test_rotate_piece(int argc,char *argv[]){
  piece pe[25]={LEAF,LEAF,LEAF,TEE,CORNER,SEGMENT,CORNER,TEE,CORNER,LEAF,TEE,TEE,SEGMENT,LEAF,LEAF,LEAF,TEE,SEGMENT,SEGMENT,CORNER,LEAF,TEE,SEGMENT,SEGMENT,LEAF};
  direction dir[25]={N,N,E,N,W,N,E,N,S,S,E,N,E,W,N,S,E,E,E,S,E,S,W,W,W};
  game g = new_game(pe,dir);
  assert(g);
  for (unsigned int x = 0;x<game_height(g);x++){
    for (unsigned int y = 0;y<game_width(g);y++){
      direction d = get_current_dir(g,x,y);
      for (int nb_rot=0;nb_rot<4;nb_rot++){
        for (int i= 0;i<nb_rot;i++){
          if (d==N){
            d=E;
          }else if (d==E){
            d=S;
          }else if (d==S){
            d=W;
          }else if (d==W){
            d=N;
          }
        }
        rotate_piece(g,x,y,nb_rot);
        if (get_current_dir(g,x,y)!=d){
          fprintf(stderr, "piece didn't rotate %d time\n",nb_rot);
          return EXIT_FAILURE;
        }
      }
    }
  }
  delete_game(g);
  return EXIT_SUCCESS;
}

int test_get_piece(int argc,char *argv[]){
  piece pe[25]={LEAF,LEAF,LEAF,TEE,CORNER,SEGMENT,CORNER,TEE,CORNER,LEAF,TEE,TEE,SEGMENT,LEAF,LEAF,LEAF,TEE,SEGMENT,SEGMENT,CORNER,LEAF,TEE,SEGMENT,SEGMENT,LEAF};
  direction dir[25]={N,N,E,N,W,N,E,N,S,S,E,N,E,W,N,S,E,E,E,S,E,S,W,W,W};
  game g = new_game(pe,dir);
  assert(g);
  for (unsigned int x=0;x<game_height(g);x++){
    for (unsigned int y=0;y<game_width(g);y++){
      if (get_piece(g,x,y)!=pe[x+y*game_height(g)]){
        fprintf(stderr, "get piece did not return the good piece\n");
        return EXIT_FAILURE;
      }
    }
  }
  delete_game(g);
  return EXIT_SUCCESS;
}

int test_restart_game(int argc,char *argv[]){
  piece pe[25]={LEAF,LEAF,LEAF,TEE,CORNER,SEGMENT,CORNER,TEE,CORNER,LEAF,TEE,TEE,SEGMENT,LEAF,LEAF,LEAF,TEE,SEGMENT,SEGMENT,CORNER,LEAF,TEE,SEGMENT,SEGMENT,LEAF};
  direction dir[25]={N,N,E,N,W,N,E,N,S,S,E,N,E,W,N,S,E,E,E,S,E,S,W,W,W};
  game g = new_game(pe,dir);
  assert(g);
  game g2 = copy_game (g);
  assert(g2);
  shuffle_dir(g);
  restart_game(g);
  for (unsigned int x=0;x<game_height(g);x++){
    for (unsigned int y=0;y<game_width(g);y++){
      if (get_current_dir(g,x,y)!=get_current_dir(g2,x,y)){
        fprintf(stderr, "piece at (%d,%d) did not correctly restarted\n",x,y);
        return EXIT_FAILURE;
      }
    }
  }
  delete_game(g);
  delete_game(g2);
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

/* ********** MAIN ROUTINE ********** */

#ifndef TESTFW

int main(int argc, char *argv[])
{
    if (argc == 1)
        usage(argc, argv);

    printf("=> RUN TEST \"%s\"\n", argv[1]);

    int status;
    if (strcmp("set_piece", argv[1]) == 0)
        status = test_set_piece(argc, argv);
    else if (strcmp("game_width", argv[1]) == 0)
        status = test_game_width(argc, argv);
    else if (strcmp("rotate_one", argv[1]) == 0)
        status = test_rotate_one(argc, argv);
    else if (strcmp("rotate_piece", argv[1]) == 0)
        status = test_rotate_piece(argc, argv);
    else if (strcmp("get_piece", argv[1]) == 0)
        status = test_get_piece(argc, argv);
    else if (strcmp("restart_game", argv[1]) == 0)
        status = test_restart_game(argc, argv);
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
