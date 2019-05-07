#include <stdio.h>
#include <stdlib.h>
#define p 2953
#define q 3727
#define STR_MAX 4294967296

typedef unsigned long long int ent;

ent * str_to_ascii(char str[], ent size){
	ent ascii[size] = {}, i=0;
	
	while (i<size){ascii[i]=str[i];i++;}
	
	return ascii;
}
/*
ent cypher (char str[], ent n, ent e){
	char * c;
	for (c=str; *c!='/0'; c=c+1){
		if (c>'ÿ'){
			printf("Error : invalide char %c in string %s",c,str);
			exit(EXIT_FAILURE);
		}
	}
	double C;
	C = (double)c;
	double ;
	C = pow(C,e)
;}
*/
int main(){
	
	
	
	for (j=0; j<size; j++){printf("%llu\n",ascii[j]);}
	
	return 0;
}