#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include "lexer.h"
#include "parser.h"

int main()
{

	openlog("ram", LOG_PID, LOG_USER);

	syslog(LOG_INFO, "RAM 1.0 Starting...");

	FILE* file_desc;

	file_desc = fopen("examples/script1.ram", "r");

	if(file_desc == NULL)
	{
		syslog(LOG_ERR, "Error opening script!");
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
		syslog(LOG_ERR, "Error closing script!");
		return 1;
	} 

	closelog();

	return 0;
}
