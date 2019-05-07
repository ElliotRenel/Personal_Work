#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "game_io.h"

#define YESYES return EXIT_SUCCESS
#define MAX_NAME_LEN 255    // Maximum charactere for NTFS files

/**
 * @brief The different commands passed as arguments translated to enum type for better use in switch statement.
 **/
typedef enum task_e {FIND_ONE, NB_SOL, FIND_ALL} task;

struct dir_doom_s{
    direction check;
    bool doomed;
    bool end;
};

typedef struct dir_doom_s* dir_doom;

dir_doom pos_to_dir(int x, int y,int w, int h, bool wrap){
    dir_doom result = (dir_doom) malloc(sizeof(struct dir_doom_s)*(NB_DIR+1));
    int index = 0;
    switch (x){
        case 0 :
            if(wrap) break;
            result[index].check = W;
            result[index].doomed = true;
            result[index].end = false;
            index++;
            break;
        default :
            if(x==w){
                result[index].check = E;
                result[index].doomed = true;
                result[index].end = false;
                index++;
            }
            result[index].check = W;
            result[index].doomed = false;
            result[index].end = false;
            index++;
            break;
    }
    switch (y){
        case 0 :
            if(wrap) break;
            result[index].check = S;
            result[index].doomed = true;
            result[index].end = false;
            index++;
            break;
        default :
            if(y==h){
                result[index].check = N;
                result[index].doomed = true;
                result[index].end = false;
                index++;
            }
            result[index].check = S;
            result[index].doomed = false;
            result[index].end = false;
            index++;
            break;
    }
    result[index].end=true;
    return result;
}

bool doomed_game(cgame g, int x, int y){
    bool wrap = is_wrapping(g);
    dir_doom to_check = pos_to_dir(x,y,game_width(g)-1,game_height(g)-1,wrap);
    for(int i=0; !to_check[i].end ;i++){
        int x2 = x,y2 = y;
        direction dir = to_check[i].check;
        near_node(g,&x2,&y2,to_check[i].check);
        bool is_e_current = is_edge_coordinates(g,x,y,dir), is_e_next = is_edge_coordinates(g,x2,y2,opposite_dir(dir)), doom = to_check[i].doomed&&!wrap;
        if((is_e_current &&( doom || !is_e_next)) || (!doom && (!is_e_current && is_e_next))){
            free(to_check);
            return true;
        }
    }
    free(to_check);
    return false;
}

/**
 * @brief Recursive function that check every possible version of the game, minus doomed version
 * @param g pointer on the game to be tested
 * @param (x,y) coordinate values of the node we currently are one
 * @param save_name string of the radical used to save what we want to save
 * @param nb_sol pointer storing the integer value of the currently known number of solution
 * @param c enum of type task indicating the task to be done 
 * @return true if found at least one solution, false if no solution where found
 **/
bool find_solution(game g, int x, int y, char* save_name, int* nb_sol, task c){
    int w = game_width((cgame)g), h = game_height((cgame)g), x2, y2;
    bool end = false;
    if(x== w-1){                    //<
        x2=0;                       //<
        y2=y+1;                     //<
    }else{                          //<      Getting the x y coordinate of the next position.
        x2=x+1;                     //<
        y2=y;                       //<
    }                               //<
    if(y2==h)
        end = true;     // Checking if we are at the end of the game.
    //int nb_before = *nb_sol;
    bool status = false;
    for(int i=0; i<NB_DIR;i++){       //<  For loop to check every
        rotate_piece_one(g,x,y);      //<  direction of piece.
        if(get_piece((cgame)g,x,y)==SEGMENT){
            i++;
        }
        if(get_piece((cgame)g,x,y)==CROSS){
            i+=NB_DIR;
        }
        if(doomed_game((cgame)g,x,y)){                   //<    Checking if upcoming version is doomed
            continue;                                    //<    to fail given what we already rotated.
        }
        if(end && is_game_over((cgame)g)){      // If we finished the version (end==true) and the game is a solution.
            switch (c){                                                                  //<
                case FIND_ONE:                                                           //<    switch to change what we do depending
                    save_game((cgame)g, save_name);                                      //<    on what task the user choose
                    (*nb_sol) = 1;                                                       //<    aka : FIND_ONE | NB_SOL | FIND_ALL
                    return true;                                                         //<
                case FIND_ALL:                                                           //<    sprintf is to add the solution number
                    (*nb_sol) = (*nb_sol)+1;                                             //<    to the file extention.
                    char new_name[MAX_NAME_LEN];                                         //<
                    sprintf(new_name, "%s%d", save_name, *nb_sol);                       //<
                    save_game((cgame)g, new_name);                                       //<    goto is used instead of break to get
                    goto exit_for;                                                       //<    out of for loop within a switch;
                case NB_SOL:                                                             //<    a break would only get out of the switch.
                    (*nb_sol) = (*nb_sol)+1;                                             //<
                    goto exit_for;                                                       //<
            }
        }
        if(!end){ 
            status = find_solution(g,x2,y2,save_name,nb_sol,c);         //   Recursive call on next coordinate.
            if(c== FIND_ONE && status)
                return status;
        }
    }
    exit_for: ;        // Tag used to break out of a for loop within a switch using a goto
    return status;             //  Returning true only if solution number increased (showing that we found another solution).
    
}

/**
 * @brief Auxiliar function to prepare the arguments for the function fin_solution and interpreting the resulte (if false)
 * @param filename string storing the name of the game to load
 * @param prefixe string storing the radical of the file we will save
 * @param c the task we need to execute
 * @return the boolean value returned by find_solution (exception: return true if task was NB_SOL)
 **/
bool aux_sol(char* filename, char* prefixe, task c){
    game g = load_game(filename);                               
    char save_name[MAX_NAME_LEN];

    switch (c){                                                                                                                                     
        case NB_SOL:                                                            //<    
            sprintf(save_name,"%s%s",prefixe,".nbsol");                         //<    
            break;                                                              //<    Assigning value for save_name 
        default:                                                                //<    depending on the task that
            sprintf(save_name,"%s%s",prefixe,".sol");                           //<    was given to us.                
            break;                                                              //<    
    }                                                                           //<
    int nb_sol = 0;
    if(!find_solution(g,0,0,save_name,&nb_sol,c) && c!=NB_SOL){                 //<                                                           
        FILE* fsave = fopen(save_name,"w");                                     //<                     
        fprintf(fsave,"NO SOLUTION");                                           //<    File saving if no solution is found.             
        fclose(fsave);                                                          //<    Except if we asked for NB_SOL.
        delete_game(g);                                                         //<    
        return false;                                                           //< 
    }                                                                           //< 
    if(c==NB_SOL){
        FILE* fsave = fopen(save_name,"w");                                     //<
        fprintf(fsave,"NB_SOL = %d",nb_sol);                                    //<    Saving solution number.
        delete_game(g);                                                         //<
        return true;                                                            //<
    }
    delete_game(g);
    return true;
}

void usage(int argc, char *argv[]){
  fprintf(stderr, "Usage: %s FIND_ONE|NB_SOL|FIND_ALL <nom_fichier_pb> <prefix_fichier_sol>\n", argv[0]);
  fprintf(stderr, "Note: <prefix_fichier_sol> cannot include a path.\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    if (argc != 4)
        usage(argc, argv);
    if(strcmp(argv[1] ,"FIND_ONE")==0){
        printf("=> Finding one solution from \"%s\"\n", argv[2]);
        clock_t begin = clock();
        aux_sol(argv[2],argv[3],FIND_ONE);
        clock_t end = clock();
        printf("\n=> Task finished and saved in \"%s.sol\" in %.3f ms\n",argv[3],(double)(end-begin)/1000.0 );
    } else if(strcmp(argv[1] ,"FIND_ALL")==0){
        printf("=> Finding all solutions from \"%s\"\n", argv[2]);
        clock_t begin = clock();
        aux_sol(argv[2],argv[3],FIND_ALL);
        clock_t end = clock();
        printf("\n=> Task finished and saved in \"%s.sol<nb_sol>\" in %.3f ms\n",argv[3],(double)(end-begin)/1000.0);
    } else if(strcmp(argv[1] ,"NB_SOL")==0){
        printf("=> Finding the number of solutions of \"%s\"\n", argv[2]);
        clock_t begin = clock();
        aux_sol(argv[2],argv[3],NB_SOL);
        clock_t end = clock();
        printf("\n=> Task finished and saved in \"%s.nbsol\" in %.3f ms\n",argv[3],(double)(end-begin)/1000.0);
    } else
        usage(argc, argv);
    YESYES;         //      What is it ? YESYES = return EXIT_SUCCESS | Why ?... CAUSE WE CAN !! 
}
