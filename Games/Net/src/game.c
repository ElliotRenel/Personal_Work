#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "game.h"

/**
 * Quick note :
 * All structures and annex functions created has been described in the given game.h  
 * All usefull comments are within functions to explain what we do
 * 
 * Certain functions don't contain comments because it has the same structure as other functions
 * Exemple : the new_game functions are all almost identical and will not have comments where it's the same
 * 
 * Also, all functions has been classified and are in a certain category so that it can be sorted properly
 */


/** STRUCTURES */

struct node_s{
  piece* p;             
  direction* d;           
};

struct game_s{
  int width;               
  int height;                 
  bool wrap;         //  Inform if the game is toric (true->it is / false->it's not)        
  node Case;         //  Node pointer containing the game actual piece and direction tabs     
  node Start;        //  Node pointer containing the game initial piece and direction tabs
};

/** INITIALISATION FUNCTIONS AND GAME MANIPULATION */

game new_game_empty(){
  game g = (game) malloc(sizeof(struct game_s));
  if(g==NULL){
    fprintf(stderr,"Error : Not enough memory\n");      //  Check for memory problem
    exit(EXIT_FAILURE);
  }
  int size = DEFAULT_SIZE*DEFAULT_SIZE;             // Default game size
  g->Case = malloc(sizeof(struct node_s));
  if(g->Case==NULL){
    fprintf(stderr,"Error : Not enough memory\n");      
    exit(EXIT_FAILURE);
  }
  g->Case->d = (direction*) malloc(size*sizeof(direction));       //  Piece and direction tab initialisation
  g->Case->p = (piece*) malloc(size*sizeof(piece));
  if(g->Case->d==NULL || g->Case->p==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Start = malloc(sizeof(struct node_s));
  if(g->Start==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  g->Start->d = (direction*) malloc(size*sizeof(direction));    // Starting nodes initialisation
  g->Start->p = (piece*) malloc(size*sizeof(piece));
  if(g->Start->d==NULL || g->Start->p==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  for(int i=0; i<size; i++){
    g->Case->p[i]=EMPTY;
    g->Case->d[i]=N;          // Affecting nodes value
    g->Start->p[i]=EMPTY;
    g->Start->d[i]=N;
  }
  g->height=DEFAULT_SIZE;
  g->width=DEFAULT_SIZE;    // Affecting default height, width and wrap
  g->wrap = false;
  return g;
}

game new_game_empty_ext(int width, int height, bool wrapping){

  // Same comment as new_game_empty

  if(width<=0 || height <=0){
    fprintf(stderr,"Error : width and height can't be null or negative\n");
    exit(EXIT_FAILURE);
  }
  game g = (game) malloc(sizeof(struct game_s));
  if(g==NULL){
    fprintf(stderr,"Error : Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  int size = width*height;                    // Game size for this particular game
  g->Case = malloc(sizeof(struct node_s));
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
  g->Start = malloc(sizeof(struct node_s));
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
  g->width=width;         // Affecting given height, width and wrap
  g->wrap = wrapping;
  return g;
}

game new_game(piece *pieces, direction *initial_directions){

  // Same comment as new_game_empty

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
  g->Case = malloc(sizeof(struct node_s));
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
  g->Start = malloc(sizeof(struct node_s));
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

  // Same comment as new_game_empty_ext

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
  g->Case = malloc(sizeof(struct node_s));
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
  g->Start = malloc(sizeof(struct node_s));
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

game copy_game (cgame g_src){

  // Code mirrors that of new_game

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
  g->Case = malloc(sizeof(struct node_s));
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
  g->Start = malloc(sizeof(struct node_s));
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
    g->Case->d[i]=g_src->Case->d[i];            // Manual initialization of the copy to prevent wrong g->start values
    g->Start->p[i]=g_src->Start->p[i];
    g->Start->d[i]=g_src->Start->d[i];
  }
  g->width = g_src->width;
  g->height = g_src->height;
  g->wrap = g_src->wrap;
  return g;
}

void delete_game (game g){

  // Pretty self explanatory

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

void set_piece(game g, int x, int y, piece piece, direction dir){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  *(g->Case->p + x + y*(g->width)) = piece;         // Translating coordinates to tab id by calculating (x + y*width),
  *(g->Case->d + x + y*(g->width)) = dir;           // then added to piece and direction pointer to directly modify it.
}

void rotate_piece_one(game game, int x, int y){
  if(game==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  game->Case->d[x+y*game->width] = (game->Case->d[x+y*game->width] + 1)%NB_DIR;     // x+y*width is the same as in set_piece, we do %NB_DIR so that W (aka 3) become N (aka 0)
}

void rotate_piece(game game, int x, int y, int nb_cw_quarter_turn){
  if(game==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  game->Case->d[x+y*game->width] = (game->Case->d[x+y*game->width] + nb_cw_quarter_turn)%NB_DIR;  // Same as in rotate_piece_one but with nb_cw_quarter_turn instead of 1
}

void set_piece_current_dir (game game, int x, int y, direction dir){
  if(game==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  game->Case->d[x+y*game->width] = dir;     // I think you understand the x+y*width by now, we use it a lot
}

void shuffle_dir(game g){
  struct timeval seed;
  gettimeofday(&seed, NULL);      // Changing the seed of rand() to the current number of microseconds on the clock (used to add a bit more randomness to shuffle)
  srand(seed.tv_usec);

  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  for(long i = 0; i<(g->width * g->height); i++){
    int set = rand()%NB_DIR;    // Affecting new random directions
    g->Case->d[i]=set;
  }
}

/** GET FUNCTIONS */

// Most of them are self explanatory 

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
  if(g==NULL || g->Case==NULL || g->Case->d==NULL){
    fprintf(stderr,"Error : Invalid game in argument\n");
    exit(EXIT_FAILURE);
  }
  if(x<0 || y<0 || x>=g->width || y>=g->height){
    fprintf(stderr,"Error : Invalid position in argument\n");
    exit(EXIT_FAILURE);
  }
  return g->Case->d[x+y*g->width];
}

direction opposite_dir(direction dir){
    return (dir+NB_DIR/2)%NB_DIR;     // Adding 2 mod NB_DIR so that N<=>S (aka 0<=>2) and E<=>W (aka 1<=>3)
}

/** ANNEX FUNCTIONS */

bool all_is_true(bool* t, int size){
  if(t==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  for(int i = 0; i<size; i++){
    if(!t[i]) return false;       // Returning false as soon as it detects a false value in t
  }
  return true;        // Return true otherwise
}

bool on_edge(cgame g, int x, int y, direction dir){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  switch(dir){
    case N:
      if(y==g->height-1) return true;     // y==g->height-1 <=> y is at its maximum height (can't go North then)
      break;
    case E:
      if(x==g->width-1) return true;      // x==g->width-1 <=> x is all the way to the right (can't go East then)
      break;            
    case S:
      if(y==0) return true;               // y==0 <=> y is at its lowest height (can't go South then)
      break;
    case W:
      if(x==0) return true;               // x==0 <=> x is all the way to the left (can't go West then)
    break;
  }
  return false;
}

void near_node(cgame g, int* x, int* y, direction dir){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  int xb = (*x), yb = (*y);     // Getting the values from the pointers

  int a=dir==E?1:0, b=dir==W?1:0, c=dir==N?1:0, d=dir==S?1:0;     // a, b, c and d are equal to either 1 or 0 depending if dir is N, E, S or W, preventing us from using a switch statement or a lot of if statement

  (*x) = (xb + a - b + g->width)%g->width;        // the +a -b are here to increment or decrement x by 1 if dir is E or W
  (*y) = (yb + c - d + g->height)%g->height;      // the +c -d are here to increment or decrement y by 1 if dir is N or S
  // The +width/height)%width/height is hear in case of wrapping, to go back to the right values (if x or y is equal to width or height or if they are negative)
}

bool is_well_paired(cgame g){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }

  bool wrap = g->wrap;
  
  for(int y=0; y<g->height; y++){
    for(int x=0; x<g->width; x++){          // Getting all game coordinates
      if(get_piece((cgame)g,x,y)==EMPTY){
        return false;                       // Checking for errors
      }
      for(direction or=N; or<NB_DIR; or++){

        bool is_e = is_edge_coordinates(g,x,y,or);
        int x2 = x, y2 = y;
        direction or2 = opposite_dir(or);

        if(is_e && (on_edge(g,x,y,or) && !wrap)){   // Checking if is_edge_coordinates(g,x,y,or) is true AND not looking at an edge in given direction (we ignore that last one if wrap is true)
          return false;
        }else{
          near_node(g,&x2,&y2,or);                  // Taking the node next to it, affecting its coordinate to x2 and y2
        }
        if(is_edge_coordinates(g,x,y,or) && !is_edge_coordinates(g,x2,y2,or2)){        // Checking if both piece can be connected to one another, returning false if not the case
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
  return g->width * g-> height;     // Self explanatory
}

bool no_loop(cgame g, int x, int y, bool* mark, direction origin){
  if(g==NULL || mark==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  int position = x + y*g->width;      // Getting index value from coordinates
  if(mark[position]){
    return false;                     // Check if we already saw this node, indicating that we looped back to it
  }else{
    mark[position]=true;              // Marking position as seen
    bool init = (x==0 && y==0);
    if(get_piece(g,x,y)!=LEAF || init){   // Checking if we aren't on a leaf (meaning we can't really go further), but also checking if we are on the origin
      for(direction d=N; d<NB_DIR; d++){
        if((d!=origin || init) && is_edge_coordinates(g,x,y,d)){          // If we can go in the direction d (is_edge_coordinates(g,x,y,d)) and d is not where we come from we continue 
          int x2 = x, y2 = y;
          near_node(g,&x2,&y2,d);               // Affecting to x2 and y2 the coordinate value of the next node
          if(!no_loop(g,x2,y2,mark,opposite_dir(d))){         // Recurrence to travel across the whole game, returning false if we ever go on an already seen node
            return false;
          }
        }
      }
    }
  }
  return true;                // Returning true when we arrive at a leaf, meaning that the whole recurrence will return true if we didn't find loops
}

/** BOOLEEN FUNCTIONS */

bool is_wrapping(cgame g){
  if(g==NULL){
    fprintf(stderr,"Error : NULL game in argument\n");
    exit(EXIT_FAILURE);
  }
  return g->wrap;
}

bool is_edge_coordinates(cgame g, int x, int y, direction dir){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  return is_edge(get_piece(g,x,y), get_current_dir(g,x,y), dir);        // Using is_edge cause it would be the same function
}

bool is_edge(piece piece, direction orientation, direction dir){        // Returns true if the direction given can be connected, false otherwise
  switch(piece){
    case LEAF:
      return orientation==dir;                                          // > : Only one possibility, same as orientation
  
    case SEGMENT:
      return ((orientation==dir)||(opposite_dir(orientation)==dir));    // - : Two possibilities, same as orientation or opposite
    
    case CORNER:
      return ((orientation==dir)||(orientation+1)%NB_DIR==dir);         // └ : Two possibilities, same orientation or next direction (clockwise)
    
    case TEE:
      return (!(dir==opposite_dir(orientation)));                       // ┴ : Three possibilities, same orientation, next direction and previous direction

    case CROSS:                                                         // + : Any direction given is valid
      return true;
    
    default:                                                            // False in case of empty node or error
      return false;
  }
}

bool is_game_over (cgame g){
  if(g==NULL){
    fprintf(stderr,"Error : NULL pointer passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  if(is_well_paired(g)){          // Checking if all game's pieces are connected to one another 
    int n = size(g);
    bool* mark= (bool*) malloc(n*sizeof(bool));
    if(mark==NULL){
      fprintf(stderr,"Error : Not enough memory\n");
      exit(EXIT_FAILURE);
    }
    for(int i=0; i<n; i++){
      mark[i]= false;
    }
    bool ret = no_loop(g,0,0,mark,S) && all_is_true(mark,n);    // Checking if the game contains loops and also if mark has any false values, meaning that some nodes haven't been checked in no_loop, meaning that g has several connected components
    free(mark);
    return ret;
  }
  return false;       // Returning false if g is not well paired
}



void afficher_game(cgame g){
  char leafs[4]={'^','>','v','<'};
  char segments[4] = {'|','-','|','-'};
  char corners[4][5] = {"└","┌","┐","┘"};
  char tees[4][5] = {"┴","├","┬","┤"};
  char* cross = "+";
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
      }else if(p==CROSS){
        printf(" %s",cross);
      }
    }
    printf("  |\n");
  }
  printf("\n _ _ _ _ _ _ _\n\n");
}