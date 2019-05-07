#include <stdio.h>


void creat_txt(char filename[], char writestr[]){
	FILE *fp;
	fp = fopen(filename,"w+");
	if ( fp ){
	   fputs(writestr,fp);
    }else{
		printf("Failed to open the file\n");
    }
	fclose(fp);
}
	

/*
int main(void){
	char string[STR_MAX];
	FILE *f = fopen("texte.txt","r");
	fgets(string, STR_MAX, f);
	
	
	fclose(f);
	
	return 0;
	
	}
*/
	
int main(){

	char * name;
	name = "Test_write.txt";
	
	char * texte;
	texte = "Si tu me lis, c'est que ça a marché";
	
	creat_txt(name, texte);
	return 0;
	
}