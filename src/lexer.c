#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "defs.h"

int ram_lexer(FILE* file_desc)
{
	char ch;
	char* lexer_buff;

	do
        {
		ch = fgetc(file_desc);

		if(is_opr(ch))
		{
			printf("Operator: %c\n", ch);
			continue;
		}

		if(is_str(ch))
		{
			lexer_buff = malloc(sizeof(char)*MAX_IDN_LEN);
			lexer_buff[0] = '\0';
			while((ch = fgetc(file_desc)) != EOF && !is_str(ch))
			{
				strncat(lexer_buff, &ch, 1);
				//TOKEN.value.is_str
				//
			}
			printf("String: %s\n", lexer_buff);
			free(lexer_buff);
			continue;
		}
        }
	while (ch != EOF);

	return 0;
}

int is_idn(char ch)
{
	if(
	(ch > 96 && ch < 123) || 
	(ch > 64 && ch < 91) ||
	ch == '_') return 1;
	return 0;
}

int is_str(char ch)
{
	if(
	ch == '"' ||
	ch == '`') return 1;
	return 0;
}

int is_opr(char ch)
{
	char operators = DEF_OPERATORS;
	int ope_len = strlen(operators);
	for(int i=0;i<ope_len;i++) {
	}
	return 0;
}
