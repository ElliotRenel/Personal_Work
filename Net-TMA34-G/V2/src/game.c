#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "game.h"

/** STRUCTURES */

struct uni_s{
  piece* p;
  direction* d;
};

struct game_s{
  int width;
  int height;
  bool wrap;
  uni Case;
  uni Start;
};

/** FONCTIONS CONSTRUCTRICES ET GAME MANIPULATION */

game new_game_empty(){
  game g = (game) malloc(sizeof(struct game_s));
  if(g==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  int size = DEFAULT_SIZE*DEFAULT_SIZE;
  g->Case = malloc(sizeof(uni));
  if(g->Case==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Case->d = (direction*) malloc(size*sizeof(direction));
  g->Case->p = (piece*) malloc(size*sizeof(piece));
  if(g->Case->d==NULL || g->Case->p==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Start = malloc(sizeof(uni));
  if(g->Start==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Start->d = (direction*) malloc(size*sizeof(direction));
  g->Start->p = (piece*) malloc(size*sizeof(piece));
  if(g->Start->d==NULL || g->Start->p==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  for(int i=0; i<size; i++){
    g->Case->p[i]=EMPTY;
    g->Case->d[i]=N;
    g->Start->p[i]=EMPTY;
    g->Start->d[i]=N;
  }
  g->height=DEFAULT_SIZE;
  g->width=DEFAULT_SIZE;
  g->wrap = false;
  return g;
}

game new_game_empty_ext(int width, int height, bool wrapping){
    if(width<=0 || height <=0){
        fprintf(stderr,"Error : width and height can't be null or negative\n");
        exit(EXIT_FAILURE);
    }
  game g = (game) malloc(sizeof(struct game_s));
  if(g==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  int size = width*height;
  g->Case = malloc(sizeof(uni));
  if(g->Case==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Case->d = (direction*) malloc(size*sizeof(direction));
  g->Case->p = (piece*) malloc(size*sizeof(piece));
  if(g->Case->d==NULL || g->Case->p==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Start = malloc(sizeof(uni));
  if(g->Start==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Start->d = (direction*) malloc(size*sizeof(direction));
  g->Start->p = (piece*) malloc(size*sizeof(piece));
  if(g->Start->d==NULL || g->Start->p==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  for(int i=0; i<size; i++){
    g->Case->p[i]=EMPTY;
    g->Case->d[i]=N;
    g->Start->p[i]=EMPTY;
    g->Start->d[i]=N;
  }
  g->height=height;
  g->width=width;
  g->wrap = wrapping;
  return g;
}

game new_game(piece *pieces, direction *initial_directions){
  if(pieces==NULL || initial_directions==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  game g = (game) malloc(sizeof(struct game_s));
  if(g==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  int size = DEFAULT_SIZE*DEFAULT_SIZE;
  g->Case = malloc(sizeof(uni));
  if(g->Case==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Case->d = (direction*) malloc(size*sizeof(direction));
  g->Case->p = (piece*) malloc(size*sizeof(piece));
  if(g->Case->d==NULL || g->Case->p==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Start = malloc(sizeof(uni));
  if(g->Start==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Start->d = (direction*) malloc(size*sizeof(direction));
  g->Start->p = (piece*) malloc(size*sizeof(piece));
  if(g->Start->d==NULL || g->Start->p==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  for(int i=0; i<size; i++){
    g->Case->p[i]=pieces[i];
    g->Case->d[i]=initial_directions[i];
    g->Start->p[i]=pieces[i];
    g->Start->d[i]=initial_directions[i];
  }
  g->width = DEFAULT_SIZE;
  g->height = DEFAULT_SIZE;
  g->wrap=false;
  return g;
}

game new_game_ext(int width, int height, piece *pieces, direction *initial_directions, bool wrapping){
    if(width<=0 || height <=0){
        fprintf(stderr,"Error : width and height can't be null or negative\n");
        exit(EXIT_FAILURE);
    }
    if(pieces==NULL || initial_directions==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  game g = (game) malloc(sizeof(struct game_s));
  if(g==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  int size = width*height;
  g->Case = malloc(sizeof(uni));
  if(g->Case==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Case->d = (direction*) malloc(size*sizeof(direction));
  g->Case->p = (piece*) malloc(size*sizeof(piece));
  if(g->Case->d==NULL || g->Case->p==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Start = malloc(sizeof(uni));
  if(g->Start==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Start->d = (direction*) malloc(size*sizeof(direction));
  g->Start->p = (piece*) malloc(size*sizeof(piece));
  if(g->Start->d==NULL || g->Start->p==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  for(int i=0; i<size; i++){
    g->Case->p[i]=pieces[i];
    g->Case->d[i]=initial_directions[i];
    g->Start->p[i]=pieces[i];
    g->Start->d[i]=initial_directions[i];
  }
  g->width = width;
  g->height = height;
  g->wrap=wrapping;
  return g;
}
}

game copy_game (cgame g_src){
    if(g_src==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
    }
  game g = (game) malloc(sizeof(struct game_s));
  if(g==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  int size = g_src->width*g_src->height;
  g->Case = malloc(sizeof(uni));
  if(g->Case==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Case->d = (direction*) malloc(size*sizeof(direction));
  g->Case->p = (piece*) malloc(size*sizeof(piece));
  if(g->Case->d==NULL || g->Case->p==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Start = malloc(sizeof(uni));
  if(g->Start==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Start->d = (direction*) malloc(size*sizeof(direction));
  g->Start->p = (piece*) malloc(size*sizeof(piece));
  if(g->Start->d==NULL || g->Start->p==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  for(int i=0; i<size; i++){
    g->Case->p[i]=g_src->Case->p[i];
    g->Case->d[i]=g_src->Case->d[i];
    g->Start->p[i]=g_src->Start->p[i];
    g->Start->d[i]=g_src->Start->d[i];
  }
  g->width = g_src->width;
  g->height = g_src->height;
  g->wrap = g_src->wrap;
  return g;
}

void delete_game (game g){
  if(g!=NULL){
    if(g->Case!=NULL){
      free(g->Case->d);
      free(g->Case->p);
    }
    free(g->Case);
    g->Case = NULL;
    if(g->Start!=NULL){
      free(g->Start->d);
      free(g->Start->p);
    }
    free(g->Start);
    g->Start = NULL;
  }
  free(g);
  g=NULL;
}

void restart_game(game g){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  for(long i = 0; i<g->width*g->height;i++){
    g->Case->d[i] = g->Start->d[i];
    g->Case->p[i] = g->Start->p[i];
  }
}

/** PIECE AND DIRECTION MODIFIER */

void set_piece(game g, int x, int y, piece piece, direction dir){}

void rotate_piece_one(game game, int x, int y){}

void rotate_piece(game game, int x, int y, int nb_ccw_quarter_turn){}

void set_piece_current_dir (game game, int x, int y, direction dir){}

void shuffle_dir(game g){}

/** FONCTIONS GET */

int game_height(cgame game){
    if(game==NULL){
        fprintf(stderr,"Error : NULL game in argument\n");
        exit(EXIT_FAILURE);
    }
    return game->height;
}

int game_width(cgame game){
    if(game==NULL){
        fprintf(stderr,"Error : NULL game in argument\n");
        exit(EXIT_FAILURE);
    }
    return game->width;
}

piece get_piece(cgame game, int x, int y){
    if(game==NULL || game->Case==NULL || game->Case->p==NULL){
        fprintf(stderr,"Error : Invalid game in argument\n");
        exit(EXIT_FAILURE);
    }
    if(x<0 || y<0 || x>=game->width || y>=game->height){
        fprintf(stderr,"Error : Invalid position in argument\n");
        exit(EXIT_FAILURE);
    }
    return game->Case->p[x+y*game->width];
}

direction get_current_dir(cgame g, int x, int y){
    if(game==NULL || game->Case==NULL || game->Case->d==NULL){
        fprintf(stderr,"Error : Invalid game in argument\n");
        exit(EXIT_FAILURE);
    }
    if(x<0 || y<0 || x>=game->width || y>=game->height){
        fprintf(stderr,"Error : Invalid position in argument\n");
        exit(EXIT_FAILURE);
    }
    return game->Case->d[x+y*game->width];
}

direction opposite_dir(direction dir){
    return (dir+NB_DIR/2)%NB_DIR;
}

/** FONCTIONS ANNEXES */

/** FONCTIONS BOOLEEN */

bool is_wrapping(cgame g){
    if(game==NULL){
        fprintf(stderr,"Error : NULL game in argument\n");
        exit(EXIT_FAILURE);
    }
    return g->wrap;
}

bool is_edge_coordinates(cgame g, int x, int y, direction dir){
    return false;
}

bool is_edge(piece piece, direction orientation, direction dir){
    return false;
}

bool is_game_over (cgame g){
    return false;
}