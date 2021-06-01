#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

int parse(TOKEN** tokens, int tokens_count)
{
	int curr = 0;
	TOKEN* previous = (void*) NULL;
	TOKEN* next = (void*) NULL;
	const char* token_types_list[] = { "STR", "LPR", "RPR", "IDN", "OPR", "LCB", "RCB", "SCN" };
	NODE nodes[tokens_count];
	while(curr < tokens_count)
        {

		//if(curr == 0)
		//if IDN
		if(!previous)
		{
			previous = tokens[curr];
		}
		if(tokens[curr]->type == IDN)
		printf("_%s %s\n", token_types_list[tokens[curr]->type], tokens[curr]->value.is_str);

		curr++;
        }

	return 0;
}

