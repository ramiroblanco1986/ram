#include <stdio.h>
#include "lexer.h"

int main()
{
	printf("RAM 1.0\n");

	FILE* file_desc;

	file_desc = fopen("script1.ram", "r");

	if(file_desc == NULL)
	{
		printf("Error 1.\n");
		return 1;
	}

	ram_lexer(file_desc);

	if(fclose(file_desc) < 0) 
	{
		printf("Error 2.\n");
		return 1;
	} 

	return 0;
}
