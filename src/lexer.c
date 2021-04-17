#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

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
			printf("_%s\n", lexer_buff);
			token = malloc(sizeof(TOKEN));
			token->type = IDN;
			token->value.is_str = calloc(strlen(lexer_buff)+1, sizeof(char));
			strcpy(token->value.is_str, lexer_buff);
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
			}
			printf("_%s\n", lexer_buff);
			token = malloc(sizeof(TOKEN));
			token->type = STR;
			token->value.is_str = calloc(strlen(lexer_buff)+1, sizeof(char));
			strcpy(token->value.is_str, lexer_buff);
			tokens_push(tokens,token);
			free(lexer_buff);
		}
		else switch(ch)
		{
			case '{':
				token = malloc(sizeof(TOKEN));
				token->type = LCB;
				tokens_push(tokens,token);
				break;
			case '}':
				token = malloc(sizeof(TOKEN));
				token->type = RCB;
				tokens_push(tokens,token);
				break;
			case '(':
				token = malloc(sizeof(TOKEN));
				token->type = LPR;
				tokens_push(tokens,token);
				break;
			case ')':
				token = malloc(sizeof(TOKEN));
				token->type = RPR;
				tokens_push(tokens,token);
				break;
			case ';':
				printf("_;\n");
				token = malloc(sizeof(TOKEN));
				token->type = SCN;
				tokens_push(tokens,token);
				break;
		}
        }
	while (ch != EOF);

	tokens_print(tokens);
	tokens_free(tokens);
	printf("END count: %i\n",tokens_count(tokens));

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
	if((p_tmp = realloc(*tokens, sizeof(TOKEN*)*( c ? c + 2 : 2 ))) == NULL) return -1;
	*tokens = p_tmp;
	*(tokens+c) = token;
	*(tokens+c+1) = malloc(sizeof(TOKEN*));
	*(tokens+c+1) = (void*)0;
	return c;
}

void tokens_print(TOKEN** tokens)
{
	int c = 0;
	const char* token_types_list[] = { "STR", "LPR", "RPR", "IDN", "OPR", "LCB", "RCB", "SCN" };
	while(*(c+tokens) != (void*)0)
	{
		switch(tokens[c]->type)
		{
			case OPR:
				printf("Token: Type: %s Value: %c\n", token_types_list[tokens[c]->type], tokens[c]->value.is_char);
				break;
			case STR:
				printf("Token: Type: %s Value: %s\n", token_types_list[tokens[c]->type], tokens[c]->value.is_str);
				break;
			case IDN:
				printf("Token: Type: %s Value: %s\n", token_types_list[tokens[c]->type], tokens[c]->value.is_str);
				break;
			case SCN:
			case LPR:
			case RPR:
			case RCB:
			case LCB:
				printf("Token: Type: %s\n", token_types_list[tokens[c]->type]);
				break;
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
