#include<stdio.h>
#include "tree.h"

extern FILE* mid;
int main(int argc, char** argv) {
	if(argc <= 1) return 1;
	FILE* f = fopen(argv[1],"r");
	if(!f)
	{
		perror(argv[1]);
		return 1;
	}
	cr = 1;
	mid = NULL;
	if(argc == 3)
	{
		mid = fopen(argv[2], "w");
	}
	yyrestart(f);
	yyparse();
//	while(yylex() != 0);
	return 0;
}
