#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef LEXER_H
#include "lexer.h"
#endif
#include "defs.h"

int ram_lexer(FILE* file_desc)
{
	char ch;
	char* lexer_buff;

	TOKEN* token = (void*)0;
	TOKEN** tokens = malloc(sizeof(TOKEN*));
	*tokens = (void*)0; 

	do
        {
		ch = fgetc(file_desc);

		if(is_opr(ch))
		{
			printf("Operator: %c\n", ch);
			token = malloc(sizeof(TOKEN));
			token->type = OPR;
			token->value.is_char = ch;
			tokens_push(tokens,token);
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
			token = malloc(sizeof(TOKEN));
			token->type = IDN;
			token->value.is_str = lexer_buff;
			tokens_push(tokens,token);
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
			token = malloc(sizeof(TOKEN));
			token->type = STR;
			token->value.is_str = lexer_buff;
			tokens_push(tokens,token);
			free(lexer_buff);
		}
        }
	while (ch != EOF);

	tokens_print(tokens);
	tokens_free(tokens);
	printf("END count______ %i\n",tokens_count(tokens));

	return 0;
}

int is_idn(char ch)
{
	int i = 0;
	char* identifiers = DEF_IDENTIFIERS;
	int idn_len = strlen(identifiers);
	for(;i<idn_len;i++) if(identifiers[i] == ch) return 1;
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

int tokens_count(TOKEN** tokens)
{
	int c = 0;
	while(*(c+tokens) != (void*)0) c++;
	return c;
}

int tokens_push(TOKEN** tokens, TOKEN* token)
{
	int c = tokens_count(tokens);
	TOKEN* p_tmp = (void*)0;
	printf("count token %i\n",c);
	if((p_tmp = realloc(*tokens, sizeof(TOKEN*)*( c ? c + 2 : 2 ))) == NULL) return -1;
	printf("after token %i\n",c ? c + 2 : 2);
	*tokens = p_tmp;
	printf("push token %i push null: %i\n",c,c+1);
	tokens[c] = token;
	tokens[c+1] = malloc(sizeof(TOKEN*));
	tokens[c+1] = (void*)0;
	return c;
}

void tokens_print(TOKEN** tokens)
{
	int c = 0;
	while(*(c+tokens) != (void*)0)
	{
		switch(tokens[c]->type)
		{
			case OPR:
				printf("Token: Type: %i Value: %c\n",tokens[c]->type,tokens[c]->value.is_char);
			case STR:
				printf("Token: Type: %i Value: %s\n",tokens[c]->type,tokens[c]->value.is_str);
			case IDN:
				printf("Token: Type: %i Value: %s\n",tokens[c]->type,tokens[c]->value.is_str);
		}
		c++;
	}
}

void tokens_free(TOKEN** tokens)
{
	int c = 0;
	do
	{
		free(*(c+tokens));
		c++;
	}
	while(*(c+tokens) != (void*)0);
}
