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
		}
		else if(is_idn(ch))
		{
			lexer_buff = malloc(sizeof(char)*MAX_IDN_LEN);
			lexer_buff[0] = '\0';
			do
			{
				strncat(lexer_buff, &ch, 1);
				ch = fgetc(file_desc);
			}
			while (is_idn(ch));
			printf("Identificador: %s\n", lexer_buff);
			free(lexer_buff);
		}
		else if(is_str(ch))
		{
			lexer_buff = malloc(sizeof(char)*MAX_STR_LEN);
			lexer_buff[0] = '\0';
			while((ch = fgetc(file_desc)) != EOF && !is_str(ch))
			{
				strncat(lexer_buff, &ch, 1);
				//TOKEN.value.is_str
			}
			printf("String: %s\n", lexer_buff);
			free(lexer_buff);
		}
        }
	while (ch != EOF);

	return 0;
}

int is_idn(char ch)
{
	int i = 0;
	char* identifiers = DEF_IDENTIFIERS;
	int idn_len = strlen(identifiers);
	for(;i<idn_len;i++) {
		if(identifiers[i] == ch) { return 1;
		 printf("_____ %c\n",identifiers[i]);
		}
	}
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
	int i = 0;
	char* operators = DEF_OPERATORS;
	int ope_len = strlen(operators);
	for(;i<ope_len;i++) if(operators[i] == ch) return 1;
	return 0;
}
