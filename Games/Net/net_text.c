#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "src/game.h"
#include "src/game_io.h"
#include "src/game_rand.h"

#define CLEARSCREEN printf("\e[1;1H\e[2J")
#define MAX_LINE_LENGH 2048

void usage_nt_dir(){
  printf("Utilisez le format demandé! (format <x> <y> avec x et y deux nombres!)\n");
}

void usage_nt_arg(){
  fprintf(stderr,"Usage : Wrong argument ! Follow the correct format for random game !\n");
  fprintf(stderr,"Usage : ./net_text <width> <height> [S|N] [3|4]\n");
}

bool check(char* str){
  if(str == NULL)
    return false;
  return true;
}

bool get_data(char *str, long *x, long *y){
  fgets(str, MAX_LINE_LENGH, stdin);
  char *token = malloc(MAX_LINE_LENGH * sizeof(char));
  token = strtok(str, " ");
  char *endptr = token;
  if(check(token)){
    *x = strtol(token, &endptr ,10);
    if((*endptr) != '\0'){
      return false;
    }
    token = strtok(NULL, " ");
    if(check(token)){
      *y = strtol(token, &endptr ,10);
      if((*endptr) != '\n'){
        return false;
      }
      token = strtok(NULL, " ");
      if(!check(token)){
        return true;
      }
    }
  }
  return false;
}

bool good_arg(char* str){
  for(int i=0; str[i]!='\0'; i++){
    if(str[i]>'9' || str[i]<'0')
      return false;
  }
  return true;
}

int main(int argc, char * argv[]){
  char gamepath[80]="";
  game g;
  switch (argc)
  {
    case 1:
      strcat(gamepath,"default.txt");
      g = load_game(gamepath);
      break;
    case 2:
      strcat(gamepath,argv[1]);
      g = load_game(gamepath);
      break;
    case 3:
      if(good_arg(argv[1]) && good_arg(argv[2]))
        g = random_game_ext(strtol(argv[1],NULL,10), strtol(argv[2],NULL,10), false, true);
      else {
        usage_nt_arg();
        return EXIT_FAILURE;
      }
      break;
    case 4:
      if(good_arg(argv[1]) && good_arg(argv[2]) && (argv[3][0]=='S' || argv[3][0]=='N'))
        g = random_game_ext(strtol(argv[1],NULL,10), strtol(argv[2],NULL,10), argv[3][0]=='S'?true:false, true);
      else {
        usage_nt_arg();
        return EXIT_FAILURE;
      }
      break;
    case 5:
      if(good_arg(argv[1]) && good_arg(argv[2]) && (argv[3][0]=='S' || argv[3][0]=='N') && (argv[4][0]=='4' || argv[4][0]=='3'))
        g = random_game_ext(strtol(argv[1],NULL,10), strtol(argv[2],NULL,10), argv[3][0]=='S', argv[4][0]=='4');
      else {
        usage_nt_arg();
        return EXIT_FAILURE;
      }
      break;
    default:
      usage_nt_arg();
      return EXIT_FAILURE;
  }
  bool formaterror = true;
  char *str = malloc(MAX_LINE_LENGH * sizeof(char));
  if(str==NULL){
    fprintf(stderr,"Not enough memory!\n");
    exit(EXIT_FAILURE);
  }
  while(!is_game_over ((cgame) g)){
    CLEARSCREEN;
    afficher_game((cgame)g);
    long x, y;
    if(formaterror == false){
      usage_nt_dir();
      formaterror = true;
    }
    printf("Coordonees de la piece a tourner (format <x> <y>): ");
    //scanf("%d %d",&x,&y);
    formaterror = get_data(str, &x, &y);
    if(formaterror && !(x<0 || y<0 || x>=game_width((cgame)g) || y>=game_height((cgame)g))){
      rotate_piece_one(g, x, y);
    }else{
      formaterror = false;
    }
  }
  afficher_game((cgame)g);
  printf("\nCONGRATS\nYOU WON!\n\n");
  delete_game(g);
  free(str);
  return EXIT_SUCCESS;
}
