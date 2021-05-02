#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

int main()
{

	printf("RAM 1.0\n");

	FILE* file_desc;

	file_desc = fopen("examples/script4.ram", "r");

	if(file_desc == NULL)
	{
		printf("Error 1.\n");
		return 1;
	}

	int tokens_count = 0;

	TOKEN** tokens = ram_lexer(file_desc, &tokens_count);

        //tokens_print(tokens, tokens_count);
        printf("END count: %i\n",tokens_count);

	parse(tokens, tokens_count);

        tokens_free(tokens, tokens_count);
        free(tokens);


	if(fclose(file_desc) < 0) 
	{
		printf("Error 2.\n");
		return 1;
	} 

	return 0;
}
