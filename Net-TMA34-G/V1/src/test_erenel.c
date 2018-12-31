#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "game.h"
#include "tools.h"

// Définition variables globale
// Tableaux pour la création d'un object game fonctionnel
piece p[25] = {LEAF,LEAF,LEAF,TEE,CORNER,SEGMENT,CORNER,TEE,CORNER,LEAF,TEE,TEE,SEGMENT,LEAF,LEAF,LEAF,TEE,SEGMENT,SEGMENT,CORNER,LEAF,TEE,SEGMENT,SEGMENT,LEAF};
direction d[25] = {N,N,E,N,W,N,E,N,S,S,E,N,E,W,N,S,E,E,E,S,E,S,W,W,W};

// Tableaux déjà fini
piece P1[25] = {LEAF,CORNER,LEAF,CORNER,CORNER,CORNER,TEE,TEE,TEE,SEGMENT,LEAF,SEGMENT,TEE,LEAF,LEAF,LEAF,SEGMENT,TEE,CORNER,LEAF,LEAF,SEGMENT,TEE,TEE,LEAF};
direction D1[25] = {N,N,W,N,W,E,S,N,W,N,E,W,W,S,S,E,E,W,N,W,E,E,S,S,W};
piece P2[25] = {CORNER,LEAF,LEAF,LEAF,LEAF,TEE,CORNER,TEE,CORNER,SEGMENT,LEAF,TEE,TEE,TEE,TEE,LEAF,TEE,LEAF,SEGMENT,SEGMENT,LEAF,SEGMENT,SEGMENT,CORNER,LEAF};
direction D2[25] = {N,W,N,N,N,E,W,E,S,S,S,E,S,N,W,E,S,W,S,S,E,E,E,S,S};
piece P3[25] = {LEAF,LEAF,LEAF,CORNER,LEAF,TEE,TEE,LEAF,TEE,TEE,CORNER,TEE,TEE,CORNER,LEAF,CORNER,CORNER,TEE,TEE,LEAF,LEAF,LEAF,CORNER,CORNER,LEAF};
direction D3[25] = {N,N,E,W,N,E,S,W,E,W,E,N,N,S,S,N,S,E,N,W,S,E,S,E,W};


/* ********** TEST NEW ********** */

int test_new(int argc, char *argv[]){
  game g = new_game(p,d);
  assert(g);
  int w = game_width(g);
  int h = game_height(g);
  if(w!=DEFAULT_SIZE || h!=DEFAULT_SIZE){
        fprintf(stderr, "Error: new_game asigned wrong width or height!\n");
        return EXIT_FAILURE;
    }
  int i  = 0;
  for(int y = 0; y<h; y++){
    for(int x = 0; x<w; x++){
      if (get_piece((cgame)g,x ,y)!= p[i] || get_current_dir((cgame)g,x ,y)!= d[i]){
        fprintf(stderr,"Error in new_game : did not affect correct piece or direction\n");
        return EXIT_FAILURE;
      }
      i++;
    }
  }
  delete_game(g);
  g = NULL;
  return EXIT_SUCCESS;
}

/* ********** TEST SET_PIECE_CURRENT_DIR ********** */

int test_set_piece_current_dir(int argc, char *argv[]){
  direction d[4] = {E,S,W,N};
  char c[4] = {'E','S','W','N'};
  game g = new_game_empty();
  assert(g);
  int w = game_width(g);
  int h = game_height(g);
  for(int y = 0; y<h; y++){
    for(int x = 0; x<w; x++){
      for(int i = 0; i<4; i++){
        set_piece_current_dir (g, x, y, d[i]);
        if(get_current_dir((cgame)g, x, y)!=d[i]) {
          fprintf(stderr,"Error in set_piece_current_dir : did not affect correct direction %c at position %d %d\n", c[i],x,y);
          return EXIT_FAILURE;
        }
      }
    }
  }
  delete_game(g);
  g = NULL;
  return EXIT_SUCCESS;
}

/* ********** TEST SHUFFLE ********** */

int test_shuffle(int argc, char *argv[]){
  game g = new_game(p,d);
  assert(g);
  int w = game_width(g);
  int h = game_height(g);
  double nb[4] = {0,0,0,0}, origin[4] ={0,0,0,0};
  for(int y = 0; y<h; y++){
    for(int x = 0; x<w; x++){
      origin[get_current_dir((cgame)g,x,y)]++;
    }
  }
  for(int i = 0; i<100; i++){
    shuffle_dir(g);
    int ind=0;
    for(int y = 0; y<h; y++){
      for(int x = 0; x<w; x++){
        nb[get_current_dir((cgame)g,x,y)]++;
        if(p[ind]!=get_piece((cgame)g,x,y)){
          fprintf(stderr,"Error in shuffle_dir : function is not supposed to modifie pieces\n");
          return EXIT_FAILURE;
        }
        ind++;
      }
    }
  }
  for(int i = 0; i<4; i++){
    nb[i] = nb[i]/2500;
    origin[i] = origin[i]/25;
  }
  if(nb[0]==origin[0] && nb[1]==origin[1] && nb[2]==origin[2] && nb[3]==origin[3]){
    fprintf(stderr,"Error in shuffle_dir : function did not shuffle directions at all\n");
    return EXIT_FAILURE;
  }
  if(nb[0]>0.35 || nb[1]>0.35 || nb[2]>0.35 || nb[3]>0.35){
    fprintf(stderr,"Error in shuffle_dir : function did not shuffle directions in a homogeneous way\n");
    return EXIT_FAILURE;
  }
  delete_game(g);
  g=NULL;
  return EXIT_SUCCESS;
}

/* ********** TEST EDGE ********** */

int mod(int a, int n){
  while(a>n){
    a = a%4;
  }
  while(a<0){
    a+=n;
  }
  return a;
}

int test_edge(int argc, char *argv[]){
  piece pe[4]={LEAF,SEGMENT, CORNER, TEE};
  direction dir[4]={N,E,S,W};
  bool cond;
  for(int a = 0; a<4; a++){
    for(int b = 0; b<4; b++){
      for(int c = 0; c<4; c++){
        switch (pe[a]) {

          case LEAF:
            cond = b==c;
            if(cond!=(is_edge(pe[a],dir[b],dir[c]))){
              fprintf(stderr,"Error in is_edge : did not returned correct bool for LEAF piece\n");
              return EXIT_FAILURE;
            }
            break;

          case SEGMENT :
            cond = (b==c) || (b==(c+2)%4);
            if(cond!=(is_edge(pe[a],dir[b],dir[c]))){
              fprintf(stderr,"Error in is_edge : did not returned correct bool for SEGMENT piece\n");
              return EXIT_FAILURE;
            }
            break;

          case CORNER :
            cond = (b==c) || (c==(b+1)%4);
            if(cond!=(is_edge(pe[a],dir[b],dir[c]))){
              fprintf(stderr,"Error in is_edge : did not returned correct bool for CORNER piece\n");
              return EXIT_FAILURE;
            }
            break;

          case TEE :
            cond = (b==c) || (c==(b+1)%4) || (c==mod(b-1,4));
            if(cond!=(is_edge(pe[a],dir[b],dir[c]))){
              fprintf(stderr,"Error in is_edge : did not returned correct bool for TEE piece\n");
              return EXIT_FAILURE;
            }
            break;

          default :
            fprintf(stderr,"Error in test_edge : this message is not supposed to show up, problem in switch\n");
            return EXIT_FAILURE;
        }
      }
    }
  }
  return EXIT_SUCCESS;
}

/* ********** TEST EDGE_COORDINATES ********** */

int test_edge_coordinate(int argc, char *argv[]){
  direction dir[4]={N,E,S,W};
  game g = new_game(p,d);
  assert(g);
  int w = game_width(g);
  int h = game_height(g);
  int i = 0;
  for(int y = 0; y<h; y++){
    for(int x = 0; x<w; x++){
      for(int n = 0; n<4; n++){
        if(is_edge(p[i],d[i],dir[n])!=is_edge_coordinates((cgame)g,x,y,dir[n])){
          fprintf(stderr,"Error in is_edge_coordinate : did not return correct boolean value\n");
          return EXIT_FAILURE;
        }
      }
      i++;
    }
  }
  delete_game(g);
  g=NULL;
  return EXIT_SUCCESS;
}

/* ********** TEST OVER ********** */

int test_over(int argc, char *argv[]){
  game g_empty = new_game_empty();

  game g1 = new_game(P1,D1);
  game g2 = new_game(P2,D2);
  game g3 = new_game(P3,D3);

  assert(g1);
  assert(g2);
  assert(g3);

  if(!is_game_over((cgame)g1) || !is_game_over((cgame)g2) || !is_game_over((cgame)g3)){
    fprintf(stderr,"Error in is_game_over : function returned false on finished game\n");
    return EXIT_FAILURE;
  }

  if(is_game_over((cgame)g_empty)){
    fprintf(stderr,"Error in is_game_over : function returned true on empty game\n");
    return EXIT_FAILURE;
  }

  rotate_piece_one(g1,rand()%5,rand()%5);
  rotate_piece_one(g2,rand()%5,rand()%5);
  rotate_piece_one(g3,rand()%5,rand()%5);

  if(is_game_over((cgame)g1) || is_game_over((cgame)g2) || is_game_over((cgame)g3)){
    fprintf(stderr, "Error in is_game_over : function returned true on unfinished game\n");
    return EXIT_FAILURE;
  }
  piece ploop[25] = {LEAF,TEE,LEAF,LEAF,LEAF,CORNER,TEE,CORNER,SEGMENT,SEGMENT,CORNER,CORNER,CORNER,CORNER,SEGMENT,LEAF,TEE,CORNER,TEE,CORNER,LEAF,TEE,TEE,TEE,LEAF};
  direction dloop[25] =  {E,N,W,N,N,N,S,W,S,S,E,W,E,S,S,E,W,N,N,S,E,S,S,S,W};
  cgame g_loop =(cgame) new_game(ploop,dloop);
  if(is_game_over(g_loop)){
    fprintf(stderr, "Error in is_game_over : function returned true on game containing loop\n");
    return EXIT_FAILURE;
  }
  piece pcc[25] = {LEAF,SEGMENT,SEGMENT,SEGMENT,LEAF,LEAF,SEGMENT,SEGMENT,SEGMENT,LEAF,LEAF,SEGMENT,SEGMENT,SEGMENT,LEAF,LEAF,SEGMENT,SEGMENT,SEGMENT,LEAF,LEAF,SEGMENT,SEGMENT,SEGMENT,LEAF};
  direction dcc[25] =  {E,E,E,E,W,E,E,E,E,W,E,E,E,E,W,E,E,E,E,W,E,E,E,E,W};
  cgame g_cc =(cgame) new_game(pcc,dcc);
  if(is_game_over(g_cc)){
    fprintf(stderr, "Error in is_game_over : function returned true on game with several connected component\n");
    return EXIT_FAILURE;
  }

  delete_game(g_empty);
  delete_game(g1);
  delete_game(g2);
  delete_game(g3);
  g1 = NULL;
  g2 = NULL;
  g3 = NULL;
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
    if (strcmp("new", argv[1]) == 0)
      status = test_new(argc, argv);
    else if (strcmp("set_piece_current_dir", argv[1]) == 0)
      status = test_set_piece_current_dir(argc, argv);
    else if (strcmp("shuffle", argv[1]) == 0)
      status = test_shuffle(argc, argv);
    else if (strcmp("edge", argv[1]) == 0)
      status = test_edge(argc, argv);
    else if (strcmp("edge_coordinate", argv[1]) == 0)
      status = test_edge_coordinate(argc, argv);
    else if (strcmp("over", argv[1]) == 0)
      status = test_over(argc, argv);
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
