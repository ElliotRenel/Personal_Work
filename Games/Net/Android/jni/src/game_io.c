#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "game_io.h"

#define MAX_LINE_LENGTH 2048

void usage_io(char *value)
{
  if (value == NULL)
  {
    fprintf(stderr, "Error: the file is not in the correct format !\n");
    exit(EXIT_FAILURE);
  }
}

piece char_to_piece(char c)
{
  switch (c)
  {
  case 'L':
    return LEAF;
  case 'S':
    return SEGMENT;
  case 'C':
    return CORNER;
  case 'T':
    return TEE;
  case 'F':
    return CROSS;
  default:
    fprintf(stderr, "Error: the file is not in the correct format !\n");
    exit(EXIT_FAILURE);
  }
}

direction char_to_direction(char c)
{
  switch (c)
  {
  case 'N':
    return N;
  case 'E':
    return E;
  case 'S':
    return S;
  case 'W':
    return W;
  default:
    fprintf(stderr, "Error: the file is not in the correct format !\n");
    exit(EXIT_FAILURE);
  }
}

game load_game(char *filename)
{
  if (filename == NULL)
  {
    fprintf(stderr, "Error: NULL string in argument !\n");
    exit(EXIT_FAILURE);
  }
  FILE *gfile;
  gfile = fopen(filename, "r");
  if (gfile == NULL)
  {
    fprintf(stderr, "Error: problem while opening file !\n");
    exit(EXIT_FAILURE);
  }
  char *str = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
  if (str == NULL)
  {
    fprintf(stderr, "Error: not enough memory !\n");
    exit(EXIT_FAILURE);
  }
  usage_io(fgets(str, MAX_LINE_LENGTH, gfile));
  char *delim = " ";
  char *strvalue;
  strvalue = strtok(str, delim);
  usage_io(strvalue);
  int width = strtol(strvalue, NULL, 10);
  strvalue = strtok(NULL, delim);
  usage_io(strvalue);
  int height = strtol(strvalue, NULL, 10);
  strvalue = strtok(NULL, delim);
  usage_io(strvalue);
  bool wrap = false;
  switch (strvalue[0])
  {
  case 'N':
    break;
  case 'S':
    wrap = true;
    break;
  default:
    usage_io(NULL);
  }

  int line_length = 3 * width;
  str = (char *)realloc(str, sizeof(char) * (line_length +1) );
  piece *p_tab = (piece *)malloc(sizeof(piece) * height * width);
  direction *d_tab = (direction *)malloc(sizeof(direction) * height * width);
  for (int y = height - 1; y > -1; y--)
  {
    fgets(str, line_length + 1, gfile);
    if (strlen(str) != line_length)
    {
      usage_io(NULL);
    }
    strvalue = strtok(str, delim);
    for (int x = 0; x < width; x++)
    {
      if (strlen(strvalue) != 2 && strvalue[2] != '\n')
      {
        usage_io(NULL);
      }
      p_tab[x + y * width] = char_to_piece(strvalue[0]);
      d_tab[x + y * width] = char_to_direction(strvalue[1]);
      strvalue = strtok(NULL, delim);
    }
  }
  game g = new_game_ext(width, height, p_tab, d_tab, wrap);
  free(str);
  free(p_tab);
  free(d_tab);
  fclose(gfile);
  return g;
}

void save_game(cgame g, char *filename){
  if (filename == NULL){
    fprintf(stderr, "Error: NULL string in argument !\n");
    exit(EXIT_FAILURE);
  }

  if (g == NULL){
    fprintf(stderr, "Error : Invalid game in argument\n");
    exit(EXIT_FAILURE);
  }

  FILE *gfile;
  gfile = fopen(filename, "w");

  if (gfile == NULL){
    fprintf(stderr, "Error: problem while opening file !\n");
    exit(EXIT_FAILURE);
  }

  int Width = game_width(g), Height = game_height(g);
  char* Wrap;
  if (is_wrapping(g))
    Wrap = "S";
  else
    Wrap = "N";
  char* p[NB_PIECE_TYPE] = {"L", "S", "C", "T", "F"};
  char* d[NB_DIR] = {"N", "E", "S", "W"};

  fprintf(gfile, "%d %d %s", Width, Height, Wrap);
  for (int y = Height-1; y >= 0; y--){
    fputs("\n", gfile);
    for (int x = 0; x < Width; x++){
      fprintf(gfile, "%s%s", p[get_piece(g,x,y)], d[get_current_dir(g,x,y)]);
      if(x!= Width-1){
        fprintf(gfile," ");
      }
    }
  }
  fputs("\n", gfile);
  fclose(gfile);
}
