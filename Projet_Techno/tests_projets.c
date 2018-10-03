#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[], char* env[]){

	char tees[4]={(char)193,(char)195,(char)194,(char)180};
	char corners[4]={(char)192,(char)218,(char)191,(char)217};
	char segments[4]={'|','-','|','-'};
	char leafs[4]={'^','>','v','<'};

	for(int i=0; i<4; i++){
		printf(" %c\n\n",tees[i]);
	}
	for(int i=0; i<4; i++){
		printf(" %c\n\n",corners[i]);
	}
	for(int i=0; i<4; i++){
		printf(" %c\n\n",segments[i]);
	}
	for(int i=0; i<4; i++){
		printf(" %c\n\n",leafs[i]);
	}

	return EXIT_SUCCESS;
}
