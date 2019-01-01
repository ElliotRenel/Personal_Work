#ifndef __CARD_H__
#define __CARD_H__

#define NUMLEN 6

typedef struct code_s* code;

code generate_c();

void delete_c(code id);

char simple_sha(int n, int times);

void print_c(code id);