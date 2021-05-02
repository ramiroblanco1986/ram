#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

int parse(TOKEN** tokens, int tokens_count)
{
	int curr = 0;
	const char* token_types_list[] = { "STR", "LPR", "RPR", "IDN", "OPR", "LCB", "RCB", "SCN" };
	while(curr < tokens_count)
        {
		printf("_%s\n", token_types_list[tokens[curr]->type]);

		//if IDN
		//if(!left) continue; 

		curr++;
        }

	return 0;
}

