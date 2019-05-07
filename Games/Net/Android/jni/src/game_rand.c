#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"


/**
 * @brief Simply a point in 2D.
 * 
 */
typedef struct point {
  int x, y;
} point;


/**
 * @brief Checks if the coordinates of the point pt is inside the grid of game g.
 */
static bool is_point_in_grid(cgame g, point pt) {
  return (pt.x >= 0 && pt.x < game_width(g)) &&
         (pt.y >= 0 && pt.y < game_height(g));
}

/**
 * @brief Returns the coordinates of the neighbour of the cell of coordinates pt
 * in the direction dir in game g. In particular, if game g is swapping the neighbour
 * of the cell of coordinates(width(g)-1, 0) is (0,0).
 * Note that coordinates returned by this function is not necessarily in the grid of g.
 */
static point get_neighbour_coordinates(cgame g, point pt, direction dir) {
  static int dx[NB_DIR] = {0, 1, 0, -1}; // increment in x depending of the direction
  static int dy[NB_DIR] = {1, 0, -1, 0}; // increment in y depending of the direction
  point neighbour_pt;
  neighbour_pt.x = pt.x + dx[dir];
  neighbour_pt.y = pt.y + dy[dir];

  if (is_wrapping(g)) {
    neighbour_pt.x = neighbour_pt.x % game_width(g);
    neighbour_pt.y = neighbour_pt.y % game_height(g);
  }
  return neighbour_pt;
}


/**
 * @brief Checks if the game contains EMPTY pieces.
 * 
 * @param g 
 * @return true if game g contains no EMPTY pieces
 * @return false if game game contains at least one EMPTY piece.
 */
static bool is_complete(cgame g) {
  for (int y = 0; y < game_height(g); y++)
    for (int x = 0; x < game_width(g); x++)
      if (get_piece(g, x, y) == EMPTY) 
	  	return false;
  return true;
}

/**
 * @brief Indicates if we can add an edge from the cell of coordinates pt toward its neighbour
 * in direction dir. (pt,dir) is a candidate edge if :
 * a) the cell of coordinates pt is not EMPTY
 * b) its neighbour in direction dir is in the grid and is EMPTY
 * c) if we do not allow CROSS piece the cell of coordinates pt must not be a TEE
 * 
 * @param pt Coordinates of a cell of game g
 * @param dir Direction to add the edge
 * @param allow_deg4 indicates if we are allow to create CROSS pieces in the game.
 */
static bool is_candidate(cgame g, point pt, direction dir, bool allow_deg4) {
  point neighbour_pt = get_neighbour_coordinates(g, pt, dir);
  if (!is_point_in_grid(g, neighbour_pt)) 
  	return false;
  return (get_piece(g, pt.x, pt.y) != EMPTY &&
          get_piece(g, neighbour_pt.x, neighbour_pt.y) == EMPTY &&
          (allow_deg4 || get_piece(g, pt.x, pt.y) != TEE));
}

/**
 * @brief Selects at random a non EMPTY cell with a direction toward an EMPTY neighbour.
 * 
 * @param g grid partially filled.
 * @param pt (output parameter): valid pointer toward a point that will contain the coordinates of the candidate
 * @param dir (output parameter): valid pointer toward a direction that will contain the direction of the candidate
 * @param allow_deg4 indicates if we are allow to create CROSS pieces in the game
 * @pre g contains at least one EMPTY cell and one non EMPTY cell
 * @post the cell at coordinates pt is not EMPTY and its neighbour in direction dir is EMPTY
 */
static void get_random_candidate(cgame g, point *p_pt, direction *p_dir,
                                 bool allow_deg4) {
  assert(!is_complete(g));
  assert(p_pt != NULL);
  assert(p_dir != NULL);
  do {
    p_pt->x = (((double)rand() * (game_width(g))) / RAND_MAX);
    p_pt->y = (((double)rand() * (game_height(g))) / RAND_MAX);
    *p_dir = (((double)rand() * (NB_DIR)) / RAND_MAX);
  } while (!is_candidate(g, *p_pt, *p_dir, allow_deg4));
}


/**
 * @brief Add an edge between the cell at coordinates pt and its neighbour in direction dir.
 * This is done by modifying the pieces of the corresponding cells.
 * For instance, if vertical segment is just at the left of an empty cell (| ) and we and an 
 * edge in the direction W, these two cells becomes a TEE and a LEAF (├<).
 */
static void add_edge(game g, point pt, direction dir);


game random_game_ext(int width, int height, bool swap, bool allow_deg4) {
  assert(width >= 3);
  assert(height >= 3);
  // Start with an empty game
  game g = new_game_empty_ext(width, height, swap);
  point pt;
  // Add an edge between two leaves (><) in the middle of the game 
  pt.x = width / 2;
  pt.y = height / 2;
  direction dir = (((double)rand() * (NB_DIR)) / RAND_MAX);
  add_edge(g, pt, dir);

  //compute a random spanning tree of the grid by adding a edge between a non-empty cell and a neighbour empty cell
  // until there is no more empty cells.
  while (!is_complete(g)) {
    direction dir;
    point pt;
    get_random_candidate(g, &pt, &dir, allow_deg4);
    add_edge(g, pt, dir);
  }
  assert(is_game_over(g));
  shuffle_dir(g);
  return g;
}

/**
 * @brief Modifies the piece of game g at coordinates pt so that it has an edge in direction dir.
 * For instance if the piece is a vertical segment (|) and we add an half-edge in direction E,
 * the piece becomes ad TEE (├) oriented E.
 */
static void add_half_edge(game g, point pt, direction dir);


static void add_edge(game g, point pt, direction dir) {
  point neighbour_pt = get_neighbour_coordinates(g, pt, dir);
  assert(is_point_in_grid(g,neighbour_pt));
  add_half_edge(g, pt, dir);
  add_half_edge(g, neighbour_pt, opposite_dir(dir));
}


/**
 * @brief A simple array of 4 booleans. We use this structure to encode for a given piece and a given orientation, if this
 * piece has an edge in the corresponding direction.
 * 
 */
typedef struct scheme {
  bool edges[NB_DIR];
} scheme;



/**
 * @brief Converts a piece together with its orientation to a 
 * scheme (an array of 4 booleans that indicates of each direction if the piece contains an edge).
 * 
 */
static scheme piece2scheme(piece piece, direction orientation) {
  scheme result;
  for (direction dir = 0; dir < NB_DIR; dir++)
    result.edges[dir] = is_edge(piece, orientation, dir);
  return result;
}

/**
 * @brief Converts a scheme into a piece together with its direction
 * 
 * @param s scheme that encodes the piece and its direction
 * @param piece (output parameter) valid pointer toward a piece corresponding to the scheme s
 * @param orientation  (output parameter) valid pointer toward a direction the the piece p.
 */
static void scheme2piece(scheme s, piece *p_piece, direction *p_orientation) {
  assert(p_piece != NULL);
  assert(p_orientation != NULL);
  int nb_edges = 0;
  for (direction dir = 0; dir < NB_DIR; dir++)
    if (s.edges[dir]) nb_edges++;

  switch (nb_edges) {
    case 0:
	  *p_piece = EMPTY;
	  *p_orientation = N;
      break;
    case 1:
      *p_piece = LEAF;
      for (direction dir = 0; dir < NB_DIR; dir++)
        if (s.edges[dir]) *p_orientation = dir;
      break;
    case 2:
      for (direction dir = 0; dir < NB_DIR; dir++) {
        if (s.edges[dir] && s.edges[(dir + 1) % NB_DIR]) {
          *p_orientation = dir;
          *p_piece = CORNER;
        }
        if (s.edges[dir] && s.edges[(dir + 2) % NB_DIR]) {
          *p_orientation = dir;
          *p_piece = SEGMENT;
        }
      }
      break;

    case 3:
      *p_piece = TEE;
      for (direction dir = 0; dir < NB_DIR; dir++)
        if (!s.edges[dir]) 
          *p_orientation = opposite_dir(dir);
      break;
    case 4:
      *p_piece = CROSS;
      *p_orientation = N;
      break;
    default:
      assert(true);
  }
  assert(*p_piece >= 0 && *p_piece < NB_PIECE_TYPE);
  assert(*p_orientation >= 0 && *p_orientation < NB_DIR);
}


static void add_half_edge(game g, point pt, direction dir) {
  // get the scheme of the piece
  scheme sch = piece2scheme(get_piece(g, pt.x, pt.y), get_current_dir(g, pt.x, pt.y));
  sch.edges[dir] = true; // Add an edge in the corresponding direction
  piece new_piece;
  direction new_orientation;
  scheme2piece(sch, &new_piece, &new_orientation); // compute the resulting piece an orientation
  set_piece(g, pt.x, pt.y, new_piece, new_orientation); // update the game
}