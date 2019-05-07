#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "src/game.h"
#include "src/game_io.h"

// Définition variables globale
// Tableaux pour la création d'un object game fonctionnel
piece p[25] = {LEAF, LEAF, LEAF, TEE, CORNER, SEGMENT, CORNER, TEE, CORNER, LEAF, TEE, TEE, SEGMENT, LEAF, LEAF, LEAF, TEE, SEGMENT, SEGMENT, CORNER, LEAF, TEE, SEGMENT, SEGMENT, LEAF};
direction d[25] = {N, N, E, N, W, N, E, N, S, S, E, N, E, W, N, S, E, E, E, S, E, S, W, W, W};

//
//
//
//
//

/* ********** TEST EMPTY ********** */

int test_empty(int argc, char *argv[])
{
  game g = new_game_empty();
  assert(g);
  int w = game_width(g);
  int h = game_height(g);
  if (w != DEFAULT_SIZE || h != DEFAULT_SIZE)
  {
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

/* ********** TEST EMPTY_EXT ********** */

int test_empty_ext(int argc, char *argv[])
{
  int w = rand() % 20 + 5;
  int h = rand() % 20 + 5;
  game g = new_game_empty_ext(w, h, true);
  assert(g);
  if (w != game_width(g) || h != game_height(g) || !is_wrapping((cgame)g))
  {
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

/* ********** TEST NEW ********** */

int test_new(int argc, char *argv[])
{
  game g = new_game(p, d);
  assert(g);
  int w = game_width(g);
  int h = game_height(g);
  if (w != DEFAULT_SIZE || h != DEFAULT_SIZE)
  {
    fprintf(stderr, "Error: new_game asigned wrong width or height!\n");
    return EXIT_FAILURE;
  }
  int i = 0;
  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      if (get_piece((cgame)g, x, y) != p[i] || get_current_dir((cgame)g, x, y) != d[i])
      {
        fprintf(stderr, "Error in new_game : did not affect correct piece or direction\n");
        return EXIT_FAILURE;
      }
      i++;
    }
  }
  delete_game(g);
  g = NULL;
  return EXIT_SUCCESS;
}

/* ********** TEST NEW_EXT ********** */

int test_new_ext(int argc, char *argv[])
{
  int w = rand() % 20 + 5;
  int h = rand() % 20 + 5;
  game g = new_game_ext(w, h, p, d, true);
  assert(g);
  if (w != game_width(g) || h != game_height(g) || !is_wrapping((cgame)g))
  {
    fprintf(stderr, "Error: new_game asigned wrong width or height!\n");
    return EXIT_FAILURE;
  }
  int i = 0;
  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      if (get_piece((cgame)g, x, y) != p[i] || get_current_dir((cgame)g, x, y) != d[i])
      {
        fprintf(stderr, "Error in new_game : did not affect correct piece or direction\n");
        return EXIT_FAILURE;
      }
      i++;
    }
  }
  delete_game(g);
  g = NULL;
  return EXIT_SUCCESS;
}

/* ******** TEST COPY ********* */

int test_copy_game(int argc, char *argv[])
{
  piece pe[25] = {LEAF, LEAF, LEAF, TEE, CORNER, SEGMENT, CORNER, TEE, CORNER, LEAF, TEE, TEE, SEGMENT, LEAF, LEAF, LEAF, TEE, SEGMENT, SEGMENT, CORNER, LEAF, TEE, SEGMENT, SEGMENT, LEAF};
  direction dir[25] = {N, N, E, N, W, N, E, N, S, S, E, N, E, W, N, S, E, E, E, S, E, S, W, W, W};
  game g = new_game(pe, dir);
  game g_copy = copy_game(g);
  int g_height = game_height(g);
  int g_width = game_width(g);
  if (!(game_height(g_copy) == g_height && game_width(g_copy) == g_width && is_wrapping((cgame)g) == is_wrapping((cgame)g_copy)))
  {
    fprintf(stderr, "Error: Height and/or width of the copy is not equal to original!");
    return EXIT_FAILURE;
  }
  for (int i = 0; i < g_width; i++)
  {
    for (int j = 0; j < g_height; j++)
    {
      if (!((get_piece(g, i, j) == get_piece(g_copy, i, j)) && (get_current_dir(g, i, j) == get_current_dir(g_copy, i, j))))
      {
        fprintf(stderr, "Error: Piece or direction is not the same! Coordonates: %d %d", i, j);
        return EXIT_FAILURE;
      }
    }
  }
  delete_game(g);
  delete_game(g_copy);

  piece pe2[16] = {LEAF, LEAF, CORNER, LEAF, SEGMENT, LEAF, CORNER, CORNER, CORNER, CROSS, CROSS, LEAF, SEGMENT, CORNER, SEGMENT, LEAF};
  direction dir2[16] = {N, E, S, N, N, N, N, S, E, N, N, W, W, S, S, E};
  g = new_game_ext(4, 4, pe2, dir2, true);
  g_copy = copy_game(g);
  g_height = game_height(g);
  g_width = game_width(g);
  if (!(game_height(g_copy) == g_height && game_width(g_copy) == g_width && is_wrapping((cgame)g) == is_wrapping((cgame)g_copy)))
  {
    fprintf(stderr, "Error: Height and/or width and/or wrapping of the copy is not equal to original!");
    return EXIT_FAILURE;
  }
  for (int i = 0; i < g_width; i++)
  {
    for (int j = 0; j < g_height; j++)
    {
      if (!((get_piece(g, i, j) == get_piece(g_copy, i, j)) && (get_current_dir(g, i, j) == get_current_dir(g_copy, i, j))))
      {
        fprintf(stderr, "Error: Piece or direction is not the same! Coordonates: %d %d", i, j);
        return EXIT_FAILURE;
      }
    }
  }
  delete_game(g);
  delete_game(g_copy);
  return EXIT_SUCCESS;
}

/* ****** TEST DELETE ********* */

int test_delete_game(int argc, char *argv[])
{
  game g = new_game_empty();
  delete_game(g);
  return EXIT_SUCCESS;
}

/* ********** TEST RESTART_GAME ********** */

int test_restart_game(int argc, char *argv[])
{
  piece pe[25] = {LEAF, LEAF, LEAF, TEE, CORNER, SEGMENT, CORNER, TEE, CORNER, LEAF, TEE, TEE, SEGMENT, LEAF, LEAF, LEAF, TEE, SEGMENT, SEGMENT, CORNER, LEAF, TEE, SEGMENT, SEGMENT, LEAF};
  direction dir[25] = {N, N, E, N, W, N, E, N, S, S, E, N, E, W, N, S, E, E, E, S, E, S, W, W, W};
  game g = new_game(pe, dir);
  assert(g);
  game g2 = copy_game(g);
  assert(g2);
  shuffle_dir(g);
  restart_game(g);
  for (unsigned int x = 0; x < game_height(g); x++)
  {
    for (unsigned int y = 0; y < game_width(g); y++)
    {
      if (get_current_dir(g, x, y) != get_current_dir(g2, x, y))
      {
        fprintf(stderr, "piece at (%d,%d) did not correctly restarted\n", x, y);
        return EXIT_FAILURE;
      }
    }
  }
  delete_game(g);
  delete_game(g2);
  return EXIT_SUCCESS;
}

//
//
//
//
//

/* ********** TEST SET_PIECE ********** */

int test_set_piece(int argc, char *argv[])
{
  direction tab_d[NB_DIR] = {N, E, S, W};
  char char_d[] = {'N', 'E', 'S', 'W'};
  piece tab_p[NB_PIECE_TYPE] = {LEAF, SEGMENT, CORNER, TEE, CROSS};
  game g = new_game_empty();
  assert(g);
  int w = game_width(g);
  int h = game_height(g);
  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      for (int i = 0; i < NB_DIR; i++)
      {
        for (int j = 0; j < NB_PIECE_TYPE; j++)
        {
          set_piece(g, x, y, tab_p[j], tab_d[i]);
          piece piece_test = get_piece(g, x, y);
          if (piece_test != tab_p[j])
          {
            fprintf(stderr, "wrong piece at (%d,%d) coordinates\n", x, y);
            return EXIT_FAILURE;
          }
          direction dir_test = get_current_dir(g, x, y);
          if (dir_test != tab_d[i])
          {
            fprintf(stderr, "direction %c at (%d,%d) coordinates was suppose to be %c\n", dir_test, x, y, char_d[i]);
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

/* ********** TEST ROTATE_ONE ********** */

int test_rotate_one(int argc, char *argv[])
{
  piece pe[25] = {LEAF, LEAF, LEAF, TEE, CORNER, SEGMENT, CORNER, TEE, CORNER, LEAF, TEE, TEE, SEGMENT, LEAF, LEAF, LEAF, TEE, SEGMENT, SEGMENT, CORNER, LEAF, TEE, SEGMENT, SEGMENT, LEAF};
  direction dir[25] = {N, N, E, N, W, N, E, N, S, S, E, N, E, W, N, S, E, E, E, S, E, S, W, W, W};
  game g = new_game(pe, dir);
  assert(g);
  for (unsigned int x = 0; x < game_height(g); x++)
  {
    for (unsigned int y = 0; y < game_width(g); y++)
    {
      direction d = get_current_dir(g, x, y);
      if (d == N)
      {
        d = E;
      }
      else if (d == E)
      {
        d = S;
      }
      else if (d == S)
      {
        d = W;
      }
      else if (d == W)
      {
        d = N;
      }
      rotate_piece_one(g, x, y);
      if (get_current_dir(g, x, y) != d)
      {
        fprintf(stderr, "piece didn't rotate once\n");
        return EXIT_FAILURE;
      }
    }
  }
  delete_game(g);
  return EXIT_SUCCESS;
}

/* ********** TEST ROTATE_PIECE ********** */

int test_rotate_piece(int argc, char *argv[])
{
  piece pe[25] = {LEAF, LEAF, LEAF, TEE, CORNER, SEGMENT, CORNER, TEE, CORNER, LEAF, TEE, TEE, SEGMENT, LEAF, LEAF, LEAF, TEE, SEGMENT, SEGMENT, CORNER, LEAF, TEE, SEGMENT, SEGMENT, LEAF};
  direction dir[25] = {N, N, E, N, W, N, E, N, S, S, E, N, E, W, N, S, E, E, E, S, E, S, W, W, W};
  game g = new_game(pe, dir);
  assert(g);
  for (unsigned int x = 0; x < game_height(g); x++)
  {
    for (unsigned int y = 0; y < game_width(g); y++)
    {
      direction d = get_current_dir(g, x, y);
      for (int nb_rot = 0; nb_rot < 4; nb_rot++)
      {
        for (int i = 0; i < nb_rot; i++)
        {
          if (d == N)
          {
            d = E;
          }
          else if (d == E)
          {
            d = S;
          }
          else if (d == S)
          {
            d = W;
          }
          else if (d == W)
          {
            d = N;
          }
        }
        rotate_piece(g, x, y, nb_rot);
        if (get_current_dir(g, x, y) != d)
        {
          fprintf(stderr, "piece didn't rotate %d time\n", nb_rot);
          return EXIT_FAILURE;
        }
      }
    }
  }
  delete_game(g);
  return EXIT_SUCCESS;
}

/* ********** TEST SET_PIECE_CURRENT_DIR ********** */

int test_set_piece_current_dir(int argc, char *argv[])
{
  direction d[4] = {E, S, W, N};
  char c[4] = {'E', 'S', 'W', 'N'};
  game g = new_game_empty();
  assert(g);
  int w = game_width(g);
  int h = game_height(g);
  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      for (int i = 0; i < 4; i++)
      {
        set_piece_current_dir(g, x, y, d[i]);
        if (get_current_dir((cgame)g, x, y) != d[i])
        {
          fprintf(stderr, "Error in set_piece_current_dir : did not affect correct direction %c at position %d %d\n", c[i], x, y);
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

int test_shuffle(int argc, char *argv[])
{
  game g = new_game(p, d);
  assert(g);
  int w = game_width(g);
  int h = game_height(g);
  double nb[4] = {0, 0, 0, 0}, origin[4] = {0, 0, 0, 0};
  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      origin[get_current_dir((cgame)g, x, y)]++;
    }
  }
  for (int i = 0; i < 100; i++)
  {
    shuffle_dir(g);
    int ind = 0;
    for (int y = 0; y < h; y++)
    {
      for (int x = 0; x < w; x++)
      {
        nb[get_current_dir((cgame)g, x, y)]++;
        if (p[ind] != get_piece((cgame)g, x, y))
        {
          fprintf(stderr, "Error in shuffle_dir : function is not supposed to modifie pieces\n");
          return EXIT_FAILURE;
        }
        ind++;
      }
    }
  }
  for (int i = 0; i < 4; i++)
  {
    nb[i] = nb[i] / 2500;
    origin[i] = origin[i] / 25;
  }
  if (nb[0] == origin[0] && nb[1] == origin[1] && nb[2] == origin[2] && nb[3] == origin[3])
  {
    fprintf(stderr, "Error in shuffle_dir : function did not shuffle directions at all\n");
    return EXIT_FAILURE;
  }
  if (nb[0] > 0.35 || nb[1] > 0.35 || nb[2] > 0.35 || nb[3] > 0.35)
  {
    fprintf(stderr, "Error in shuffle_dir : function did not shuffle directions in a homogeneous way\n");
    return EXIT_FAILURE;
  }
  delete_game(g);
  g = NULL;
  return EXIT_SUCCESS;
}

//
//
//
//
//

/* ******** TEST GAME_HEIGHT ***** */

int test_game_height(int argc, char *argv[])
{
  game g = new_game_empty();
  //printf("Height of empty: Is it equal to DEFAULT_SIZE? %s\n", game_height(g)==DEFAULT_SIZE ? " Yes!\n" : " No!\n");
  piece pe[25] = {LEAF, LEAF, LEAF, TEE, CORNER, SEGMENT, CORNER, TEE, CORNER, LEAF, TEE, TEE, SEGMENT, LEAF, LEAF, LEAF, TEE, SEGMENT, SEGMENT, CORNER, LEAF, TEE, SEGMENT, SEGMENT, LEAF};
  direction dir[25] = {N, N, E, N, W, N, E, N, S, S, E, N, E, W, N, S, E, E, E, S, E, S, W, W, W};
  game g2 = new_game(pe, dir);
  //printf("Height of g2: Is it equal to DEFAULT_SIZE?%s\n", game_height(g2)==DEFAULT_SIZE ? " Yes!\n" : " No!\n");
  if (game_height(g2) != DEFAULT_SIZE || game_height(g) != DEFAULT_SIZE)
  {
    fprintf(stderr, "Height not equal to DEFAULT_SIZE!");
    return EXIT_FAILURE;
  }
  int h = (rand() % 20) + 5;
  int w = (rand() % 20) + 5;
  game grandom = new_game_empty_ext(w, h, false);
  if (game_height(grandom) != h)
  {
    fprintf(stderr, "the height of the game was supposed to be %d and not %d\n", w, game_width(g2));
    return EXIT_FAILURE;
  }
  delete_game(g);
  delete_game(g2);
  delete_game(grandom);
  return EXIT_SUCCESS;
}

/* ********** TEST GAME_WIDTH ********** */

int test_game_width(int argc, char *argv[])
{
  game g = new_game_empty();
  assert(g);
  if (game_width(g) != DEFAULT_SIZE)
  {
    fprintf(stderr, "the width of the game was supposed to be %d and not %d\n", DEFAULT_SIZE, game_width(g));
    return EXIT_FAILURE;
  }
  //printf("width of empty:%d\n", game_width(g));
  piece pe[25] = {LEAF, LEAF, LEAF, TEE, CORNER, SEGMENT, CORNER, TEE, CORNER, LEAF, TEE, TEE, SEGMENT, LEAF, LEAF, LEAF, TEE, SEGMENT, SEGMENT, CORNER, LEAF, TEE, SEGMENT, SEGMENT, LEAF};
  direction dir[25] = {N, N, E, N, W, N, E, N, S, S, E, N, E, W, N, S, E, E, E, S, E, S, W, W, W};
  game g2 = new_game(pe, dir);
  assert(g2);
  if (game_width(g2) != DEFAULT_SIZE)
  {
    fprintf(stderr, "the width of the game was supposed to be %d and not %d\n", DEFAULT_SIZE, game_width(g2));
    return EXIT_FAILURE;
  }
  //printf("width of g2:%d (5x5)\n", game_width(g2));
  int h = (rand() % 20) + 5;
  int w = (rand() % 20) + 5;
  game grandom = new_game_empty_ext(w, h, false);
  if (game_width(grandom) != w)
  {
    fprintf(stderr, "the width of the game was supposed to be %d and not %d\n", w, game_width(g2));
    return EXIT_FAILURE;
  }
  delete_game(g);
  delete_game(g2);
  delete_game(grandom);
  return EXIT_SUCCESS;
}

/* ********** TEST GET_PIECE ********** */

int test_get_piece(int argc, char *argv[])
{
  piece pe[25] = {LEAF, LEAF, LEAF, TEE, CORNER, SEGMENT, CORNER, TEE, CORNER, LEAF, TEE, TEE, SEGMENT, LEAF, LEAF, LEAF, TEE, SEGMENT, SEGMENT, CORNER, LEAF, TEE, SEGMENT, SEGMENT, LEAF};
  direction dir[25] = {N, N, E, N, W, N, E, N, S, S, E, N, E, W, N, S, E, E, E, S, E, S, W, W, W};
  game g = new_game(pe, dir);
  assert(g);
  for (unsigned int x = 0; x < game_height(g); x++)
  {
    for (unsigned int y = 0; y < game_width(g); y++)
    {
      if (get_piece(g, x, y) != pe[x + y * game_height(g)])
      {
        fprintf(stderr, "get piece did not return the good piece\n");
        return EXIT_FAILURE;
      }
    }
  }
  delete_game(g);
  return EXIT_SUCCESS;
}

/* *********** TEST CURRENT_DIR ********** */

int test_get_current_dir(int argc, char *argv[])
{
  game g = new_game_empty();
  for (int i = 0; i < game_height(g); i++)
  {
    for (int j = 0; j < game_width(g); j++)
    {
      if (get_current_dir(g, j, i) != N)
      {
        return EXIT_FAILURE;
      }
    }
  }
  game g2 = new_game(p, d);
  int n = 0;
  for (int i = 0; i < game_height(g2); i++)
  {
    for (int j = 0; j < game_width(g2); j++)
    {
      if (get_current_dir(g2, j, i) != d[n])
      {
        return EXIT_FAILURE;
      }
      n++;
    }
  }
  delete_game(g);
  delete_game(g2);
  return EXIT_SUCCESS;
}

/* ***** TEST OPPOSITE_DIR ***** */

int test_opposite_dir(int argc, char *argv[])
{ /* Appel avec opposite_dir */
  direction test_tab[4] = {N, E, S, W};
  direction opposite_dir_tab[4] = {S, W, N, E};
  for (int i = 0; i < 4; i++)
  {
    if (opposite_dir(test_tab[i]) != opposite_dir_tab[i])
    {
      fprintf(stderr, "Error: Opposite direction of %c is not %c!", test_tab[i], opposite_dir_tab[i]);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

//
//
//
//
//

int mod(int a, int n)
{
  while (a > n)
  {
    a = a % 4;
  }
  while (a < 0)
  {
    a += n;
  }
  return a;
}

/* ********** TEST WRAPPING ********** */

int test_wrapping(int argc, char *argv[])
{
  game g = new_game_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, true);
  assert(g);
  if (!is_wrapping(g))
  {
    fprintf(stderr, "Error in is_wrapping : returned false on wrapped game\n");
    return EXIT_FAILURE;
  }
  delete_game(g);
  g = new_game_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, false);
  assert(g);
  if (is_wrapping(g))
  {
    fprintf(stderr, "Error in is_wrapping : returned true on unwrapped game\n");
    return EXIT_FAILURE;
  }
  delete_game(g);
  return EXIT_SUCCESS;
}

/* ********** TEST EDGE ********** */

int test_edge(int argc, char *argv[])
{
  piece pe[NB_PIECE_TYPE] = {LEAF, SEGMENT, CORNER, TEE, CROSS};
  direction dir[4] = {N, E, S, W};
  bool cond;
  for (int a = 0; a < NB_PIECE_TYPE; a++)
  {
    for (int b = 0; b < NB_DIR; b++)
    {
      for (int c = 0; c < NB_DIR; c++)
      {
        switch (pe[a])
        {

        case LEAF:
          cond = b == c;
          if (cond != (is_edge(pe[a], dir[b], dir[c])))
          {
            fprintf(stderr, "Error in is_edge : did not returned correct bool for LEAF piece\n");
            return EXIT_FAILURE;
          }
          break;

        case SEGMENT:
          cond = (b == c) || (b == (c + 2) % 4);
          if (cond != (is_edge(pe[a], dir[b], dir[c])))
          {
            fprintf(stderr, "Error in is_edge : did not returned correct bool for SEGMENT piece\n");
            return EXIT_FAILURE;
          }
          break;

        case CORNER:
          cond = (b == c) || (c == (b + 1) % 4);
          if (cond != (is_edge(pe[a], dir[b], dir[c])))
          {
            fprintf(stderr, "Error in is_edge : did not returned correct bool for CORNER piece\n");
            return EXIT_FAILURE;
          }
          break;

        case TEE:
          cond = (b == c) || (c == (b + 1) % 4) || (c == mod(b - 1, 4));
          if (cond != (is_edge(pe[a], dir[b], dir[c])))
          {
            fprintf(stderr, "Error in is_edge : did not returned correct bool for TEE piece\n");
            return EXIT_FAILURE;
          }
          break;

        case CROSS:
          if (!is_edge(pe[a], dir[b], dir[c]))
          {
            fprintf(stderr, "Error in is_edge : did not returned correct bool for TEE piece\n");
            return EXIT_FAILURE;
          }
          break;

        default:
          fprintf(stderr, "Error in test_edge : this message is not supposed to show up, problem in switch\n");
          return EXIT_FAILURE;
        }
      }
    }
  }
  return EXIT_SUCCESS;
}

/* ********** TEST EDGE_COORDINATES ********** */

int test_edge_coordinate(int argc, char *argv[])
{
  direction dir[4] = {N, E, S, W};
  game g = new_game(p, d);
  assert(g);
  int w = game_width(g);
  int h = game_height(g);
  int i = 0;
  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      for (int n = 0; n < 4; n++)
      {
        if (is_edge(p[i], d[i], dir[n]) != is_edge_coordinates((cgame)g, x, y, dir[n]))
        {
          fprintf(stderr, "Error in is_edge_coordinate : did not return correct boolean value\n");
          return EXIT_FAILURE;
        }
      }
      i++;
    }
  }
  delete_game(g);
  g = NULL;
  return EXIT_SUCCESS;
}

/* ********** TEST OVER ********** */

int test_over(int argc, char *argv[])
{
  game g_empty = new_game_empty();

  game g1 = load_game("game_test_files/over1.txt");
  game g2 = load_game("game_test_files/over2.txt");
  game g3 = load_game("game_test_files/over3.txt");

  assert(g1);
  assert(g2);
  assert(g3);

  if (!is_game_over((cgame)g1) || !is_game_over((cgame)g2) || !is_game_over((cgame)g3))
  {
    fprintf(stderr, "Error in is_game_over : function returned false on finished game\n");
    return EXIT_FAILURE;
  }

  if (is_game_over((cgame)g_empty))
  {
    fprintf(stderr, "Error in is_game_over : function returned true on empty game\n");
    return EXIT_FAILURE;
  }

  rotate_piece_one(g1, rand() % 5, rand() % 5);
  rotate_piece_one(g2, rand() % 5, rand() % 5);
  rotate_piece_one(g3, rand() % 5, rand() % 5);

  if (is_game_over((cgame)g1) || is_game_over((cgame)g2) || is_game_over((cgame)g3))
  {
    fprintf(stderr, "Error in is_game_over : function returned true on unfinished game\n");
    return EXIT_FAILURE;
  }
  piece ploop[25] = {LEAF, TEE, LEAF, LEAF, LEAF, CORNER, TEE, CORNER, SEGMENT, SEGMENT, CORNER, CORNER, CORNER, CORNER, SEGMENT, LEAF, TEE, CORNER, TEE, CORNER, LEAF, TEE, TEE, TEE, LEAF};
  direction dloop[25] = {E, N, W, N, N, N, S, W, S, S, E, W, E, S, S, E, W, N, N, S, E, S, S, S, W};
  game g_loop = (game)new_game(ploop, dloop);
  if (is_game_over(g_loop))
  {
    fprintf(stderr, "Error in is_game_over : function returned true on game containing loop\n");
    return EXIT_FAILURE;
  }
  piece pcc[25] = {LEAF, SEGMENT, SEGMENT, SEGMENT, LEAF, LEAF, SEGMENT, SEGMENT, SEGMENT, LEAF, LEAF, SEGMENT, SEGMENT, SEGMENT, LEAF, LEAF, SEGMENT, SEGMENT, SEGMENT, LEAF, LEAF, SEGMENT, SEGMENT, SEGMENT, LEAF};
  direction dcc[25] = {E, E, E, E, W, E, E, E, E, W, E, E, E, E, W, E, E, E, E, W, E, E, E, E, W};
  game g_cc = (game)new_game(pcc, dcc);
  if (is_game_over(g_cc))
  {
    fprintf(stderr, "Error in is_game_over : function returned true on game with several connected component\n");
    return EXIT_FAILURE;
  }
  piece pw1[16] = {LEAF, LEAF, CORNER, LEAF, SEGMENT, LEAF, CORNER, CORNER, CORNER, CROSS, CROSS, LEAF, SEGMENT, CORNER, SEGMENT, LEAF};
  direction dw1[16] = {N, E, S, N, N, N, N, S, E, N, N, W, W, S, S, E};
  game gw1 = new_game_ext(4, 4, pw1, dw1, true);
  if (!is_game_over((cgame)gw1))
  {
    fprintf(stderr, "Error in is_game_over : function returned false on finished game\n");
    return EXIT_FAILURE;
  }
  piece pw2[25] = {LEAF, SEGMENT, CORNER, LEAF, LEAF, LEAF, LEAF, TEE, CORNER, TEE, LEAF, CORNER, TEE, SEGMENT, TEE, CORNER, TEE, TEE, CORNER, LEAF, LEAF, LEAF, TEE, TEE, LEAF};
  direction dw2[25] = {E, E, S, N, N, W, E, N, S, E, E, W, E, E, W, N, S, S, W, S, S, E, N, S, W};
  game gw2 = new_game_ext(5, 5, pw2, dw2, false);
  if (is_game_over((cgame)gw2))
  {
    fprintf(stderr, "Error in is_game_over : function returned true on invalid finished game\n");
    return EXIT_FAILURE;
  }

  delete_game(g_empty);
  delete_game(g1);
  delete_game(g2);
  delete_game(g3);
  delete_game(g_loop);
  delete_game(g_cc);
  delete_game(gw1);
  delete_game(gw2);
  return EXIT_SUCCESS;
}

/* ******* TEST LOAD ******* */


int test_load(int argc, char *argv[]){
  piece pe[12] = {LEAF, CORNER, LEAF, LEAF, CORNER, TEE, TEE, TEE, LEAF, LEAF, CORNER, CORNER};
  direction dir[12] = {N, E, S, W, W, E, E, W, N, N, S, S};
  game g1 = new_game_ext(3, 4, pe, dir, false);
  game g2 = load_game("../game_files/test_load.txt");
  if(game_height((cgame)g1)==game_height((cgame)g2) && game_width((cgame)g1)==game_width((cgame)g2) && is_wrapping((cgame)g1)==is_wrapping((cgame)g2)){
    for(int i =0; i<game_width((cgame)g1); i++){
      for(int j=0; j<game_height((cgame)g1); j++){
        if(get_piece((cgame)g1,i,j) != get_piece((cgame)g2,i,j) || get_current_dir((cgame)g1, i, j) != get_current_dir((cgame)g2, i, j)){
          delete_game(g1);
          delete_game(g2);
          return EXIT_FAILURE;
        }
      }
    }
    delete_game(g1);
    delete_game(g2);
    return EXIT_SUCCESS;
  }
  delete_game(g1);
  delete_game(g2);
  return EXIT_FAILURE;
}

/* ****** TEST SAVE ****** */

int test_save(int argc, char *argv[]){
  piece pe[12] = {LEAF, CORNER, LEAF, LEAF, CORNER, TEE, TEE, TEE, LEAF, LEAF, CORNER, CORNER};
  direction dir[12] = {N, E, S, W, W, E, E, W, N, N, S, S};
  game g1 = new_game_ext(3, 4, pe, dir, false);
  save_game(g1,"game_test_files/test_save.txt");
  game g2 = load_game("game_test_files/test_save.txt");
  if(game_height((cgame)g1)==game_height((cgame)g2) && game_width((cgame)g1)==game_width((cgame)g2) && is_wrapping((cgame)g1)==is_wrapping((cgame)g2)){
    for(int i =0; i<game_width((cgame)g1); i++){
      for(int j=0; j<game_height((cgame)g1); j++){
        if(get_piece((cgame)g1,i,j) != get_piece((cgame)g2,i,j) || get_current_dir((cgame)g1, i, j) != get_current_dir((cgame)g2, i, j)){
          delete_game(g1);
          delete_game(g2);
          return EXIT_FAILURE;
        }
      }
    }
    delete_game(g1);
    delete_game(g2);
    return EXIT_SUCCESS;
  }
  delete_game(g1);
  delete_game(g2);
  return EXIT_FAILURE;
}
//
//
//
//
//

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
  else if (strcmp("set_piece", argv[1]) == 0)
    status = test_set_piece(argc, argv);
  else if (strcmp("empty", argv[1]) == 0)
    status = test_empty(argc, argv);
  else if (strcmp("opposite_dir", argv[1]) == 0)
    status = test_opposite_dir(argc, argv);
  else if (strcmp("copy_game", argv[1]) == 0)
    status = test_copy_game(argc, argv);
  else if (strcmp("game_height", argv[1]) == 0)
    status = test_game_height(argc, argv);
  else if (strcmp("delete_game", argv[1]) == 0)
    status = test_delete_game(argc, argv);
  else if (strcmp("get_current_dir", argv[1]) == 0)
    status = test_get_current_dir(argc, argv);
  else if (strcmp("new_ext", argv[1]) == 0)
    status = test_new_ext(argc, argv);
  else if (strcmp("empty_ext", argv[1]) == 0)
    status = test_empty_ext(argc, argv);
  else if (strcmp("wrapping", argv[1]) == 0)
    status = test_wrapping(argc, argv);
  else if (strcmp("load", argv[1]) == 0)
    status = test_load(argc, argv);
  else if (strcmp("save", argv[1]) == 0)
    status = test_save(argc, argv);
  else
  {
    fprintf(stderr, "Error: test %s not found!\n", argv[1]);
    return EXIT_FAILURE;
  }

  if (status != EXIT_SUCCESS)
    printf("FAILURE (status %d)\n", status);
  else
    printf("SUCCESS (status %d)\n", status);

  return status;
}

#endif
