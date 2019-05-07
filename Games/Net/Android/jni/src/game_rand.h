#ifndef GAME_RAND_H
#define GAME_RAND_H

#include "game.h"

/**
 * @brief Computes a random game. This function insures that every game of a given size 
 * can be generate, but not necessarily with the same probability.
 * @param width width of the game
 * @param height height of the game
 * @param allow_deg4 Indicates if the generated game is allowed to have CROSS pieces
 * @return a random game with directions of pieces are shuffled.
 * @pre width >= 3 and height >= 3
 * @post the result is a game that admits at least one solution.
 **/
game random_game_ext(int width, int height, bool swap, bool allow_deg4);

#endif  // GAME_RAND
