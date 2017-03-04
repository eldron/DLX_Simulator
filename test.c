#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** args){
	FILE * fin = fopen(args[1], "r");
	char s[100];
	while(fgets(s, 100, fin) != NULL){
		//printf("%s", s);
	}
	fclose(fin);
	printf("%s", s);
	printf("%d\n", strlen(s));
	printf("%d\n", atoi("-123;"));
	return 0;
}