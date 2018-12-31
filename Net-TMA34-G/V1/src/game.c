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
  return g;
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

void set_piece(game g, int x, int y, piece piece, direction orientation){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  *(g->Case->p + x + y*(g->width)) = piece;
  *(g->Case->d + x + y*(g->width)) = orientation;
}

void rotate_piece_one(game game, int x, int y){
  if(game==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  game->Case->d[x+y*game->width] = (game->Case->d[x+y*game->width] + 1)%NB_DIR; 
}

void rotate_piece(game game, int x, int y, int nb_cw_quarter_turn){
  if(game==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  game->Case->d[x+y*game->width] = (game->Case->d[x+y*game->width] + nb_cw_quarter_turn)%NB_DIR;
}

void set_piece_current_dir (game game, int x, int y, direction dir){
  if(game==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  game->Case->d[x+y*game->width] = dir;
}

void shuffle_dir(game g){
  struct timeval seed;
  gettimeofday(&seed, NULL);
  srand(seed.tv_usec);
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  for(long i = 0; i<(g->width * g->height); i++){
    int set = rand()%NB_DIR;
    g->Case->d[i]=set;
  }
}

/** FONCTIONS GET */

int game_height(cgame game){
  if(game==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  return game->height;
}

int game_width(cgame game){
  if(game==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  return game->width;
}

piece get_piece(cgame g, int x, int y){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  return g->Case->p[x+y*g->width];
}

direction get_current_dir(cgame g, int x, int y){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  return g->Case->d[x+y*g->width];
}

direction opposite_dir(direction dir){
  return (dir+NB_DIR/2)%NB_DIR;
}

/** FONCTIONS ANNEXES */

bool all_is_true(bool* t, int size){
  if(t==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  for(int i = 0; i<size; i++){
    if(!t[i]) return false;
  }
  return true;
}

bool on_edge(cgame g, int x, int y, direction dir){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  switch(dir){
    case N:
      if(y==g->height-1) return true;
      break;
    case E:
      if(x==g->width-1) return true;
      break;            
    case S:
      if(y==0) return true;
      break;
    case W:
      if(x==0) return true;
    break;
  }
  return false;
}

void near_piece(cgame g, int* x, int* y, direction dir){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  switch(dir){
    case N:
      (*y)++;
      break;
    case E:
      (*x)++;
      break;            
    case S:
      (*y)--;
      break;
    case W:
      (*x)--;
    break;
  }
}

bool is_well_paired(cgame g){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  for(int y=0; y<g->height; y++){
    for(int x=0; x<g->width; x++){
      if(get_piece((cgame)g,x,y)==EMPTY){
        return false;
      }
      for(direction or=N; or<NB_DIR; or++){
        bool is_e = is_edge_coordinates(g,x,y,or);
        int x2 = x, y2 = y;
        direction or2 = opposite_dir(or);
        if(is_e && on_edge(g,x,y,or)){
          return false;
        }else{
          near_piece(g,&x2,&y2,or);
        }
        if(is_edge_coordinates(g,x,y,or) && !is_edge_coordinates(g,x2,y2,or2)){
          return false;
        }
      }
    }
  }
  return true;
}

int size(cgame g){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  return g->width * g-> height;
}

bool no_loop(cgame g, int x, int y, bool* mark, direction origin){
  if(g==NULL || mark==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  int position = x + y*g->width;
  if(mark[position]){
    return false;
  }else{
    mark[position]=true;
    if(get_piece(g,x,y)!=LEAF || (x==0 && y==0)){
      for(direction d=N; d<NB_DIR; d++){
        if(d!=origin && is_edge_coordinates(g,x,y,d)){
          int x2 = x, y2 = y;
          near_piece(g,&x2,&y2,d);
          if(!no_loop(g,x2,y2,mark,opposite_dir(d))){
            return false;
          }
        }
      }
    }
  }
  return true;
}

/** FONCTIONS BOOLEEN */

bool is_edge_coordinates(cgame g, int x, int y, direction dir){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  return is_edge(get_piece(g,x,y), get_current_dir(g,x,y), dir);
}

bool is_edge(piece piece, direction orientation, direction dir){
  switch(piece){
    case LEAF:
      return orientation==dir;
  
    case SEGMENT:
      return ((orientation==dir)||(opposite_dir(orientation)==dir));
    
    case CORNER:
      return ((orientation==dir)||(orientation+1)%NB_DIR==dir);
    
    case TEE:
      return (!(dir==opposite_dir(orientation)));
    
    default:
      return false;
  }
}

bool is_game_over (cgame g){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  if(is_well_paired(g)){
    int n = size(g);
    bool* mark= (bool*) malloc(n*sizeof(bool));
    if(mark==NULL){
      fprintf(stderr,"Error : Not enough memory\n");
      exit(EXIT_FAILURE);
    }
    for(int i=0; i<n; i++){
      mark[i]= false;
    }
    bool ret = no_loop(g,0,0,mark,S) && all_is_true(mark,n);
    free(mark);
    return ret;
  }
  return false;
}