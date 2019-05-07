#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#define NUMLEN 6

typedef struct code_s* code;

struct code_s {
    int sec_num;
    char sec_char;
    int* id_num;
};

char simple_sha(int n, int times){
    for(int i=0; i<times; i++){
        n = (n*n + n*times)%26;
    }
    char c = (char)(n + (int)'A');
    return c;
}

code generate_c(){
    struct timeval seed;
    gettimeofday(&seed, NULL);
    srand(seed.tv_usec);
    code id = (code)malloc(sizeof(struct code_s));
    if(id==NULL){
        fprintf(stderr,"Error : Not enough memory\n");
        exit(EXIT_FAILURE);
    }
    id->id_num = (int*)malloc(sizeof(int)*NUMLEN);
    if(id->id_num==NULL){
        fprintf(stderr,"Error : Not enough memory\n");
        exit(EXIT_FAILURE);
    }
    int total = 0;
    for(int i=0; i<NUMLEN; i++){
        id->id_num[i] = rand()%10;
        total += id->id_num[i];
    }
    id->sec_num = (total*total+total)%10;
    id->sec_char = simple_sha(id->sec_num,total);
    return id;
}

void delete_c(code id){
    if(id!=NULL){
        if(id->id_num!=NULL){
            free(id->id_num);
            id->id_num = NULL;
        }
        free(id);
    }
}

void print_c(code id){
    if(id==NULL){
        fprintf(stderr,"Error : NULL code in arguments\n");
        exit(EXIT_FAILURE);
    }
    printf("%d%c-",id->sec_num,id->sec_char);
    for(int i=0; i<NUMLEN; i++){
        printf("%d",id->id_num[i]);
    }
}

void main(void){
    code carte = generate_c();
    print_c(carte);
    delete_c(carte);
}