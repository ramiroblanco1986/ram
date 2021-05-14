#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

TOKEN** ram_lexer(FILE* file_desc, int* tokens_count)
{
	char ch;
	char* lexer_buff;

	int array_size = ARR_START_SIZE;

	TOKEN* token = (void*)0;
	TOKEN** tokens = malloc(sizeof(TOKEN*) * array_size);

	do
        {
		ch = fgetc(file_desc);

		if(is_opr(ch))
		{
			//printf("es ope: %c\n",ch);
			token = (TOKEN*) malloc(sizeof(TOKEN));
			token->type = OPR;
			token->value.is_char = ch;
		}
		else if(is_str(ch))
		{
			token = (TOKEN*) malloc(sizeof(TOKEN));
			lexer_buff = malloc(sizeof(char)*MAX_STR_LEN);
			lexer_buff[0] = '\0';
			while((ch = fgetc(file_desc)) != EOF && !is_str(ch))
			{
				strncat(lexer_buff, &ch, 1);
			}
			token->type = STR;
			token->value.is_str = calloc(strlen(lexer_buff)+1, sizeof(char));
			strcpy(token->value.is_str, lexer_buff);
			free(lexer_buff);
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
			ungetc (ch, file_desc);
			token = (TOKEN*) malloc(sizeof(TOKEN));
			token->type = IDN;
			token->value.is_str = calloc(strlen(lexer_buff)+1, sizeof(char));
			strcpy(token->value.is_str, lexer_buff);
			free(lexer_buff);
		}
		else switch(ch)
		{
			case '{':
				token = malloc(sizeof(TOKEN));
				token->type = LCB;
				break;
			case '}':
				token = malloc(sizeof(TOKEN));
				token->type = RCB;
				break;
			case '(':
				token = malloc(sizeof(TOKEN));
				token->type = LPR;
				break;
			case ')':
				token = malloc(sizeof(TOKEN));
				token->type = RPR;
				break;
			case ';':
				token = malloc(sizeof(TOKEN));
				token->type = SCN;
				break;
		}
		
		if(token)
		{
			tokens = tokens_push(tokens,token,tokens_count,&array_size);
			if(!tokens)
			{
				perror("Error push!\n");
				exit(1);
			}
			token = (void*)0;
		}
        }
	while (ch != EOF);

	return tokens;
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

TOKEN** tokens_push(TOKEN** tokens, TOKEN* token, int* tokens_count, int* array_size)
{

	TOKEN* p_tmp;

	if(*array_size == *tokens_count)
	{
		*array_size = *array_size + ARR_GF;
		if((p_tmp = realloc(tokens, sizeof(TOKEN*)* *array_size)) == NULL) return 0;
		tokens = (TOKEN**)p_tmp;
	}

	tokens[*tokens_count] = token;

	(*tokens_count)++;

	return tokens;
}

void tokens_print(TOKEN** tokens, int tokens_count)
{
	int c = 0;
	const char* token_types_list[] = { "STR", "LPR", "RPR", "IDN", "OPR", "LCB", "RCB", "SCN" };
	while(c < tokens_count)
	{
		switch(tokens[c]->type)
		{
			case OPR:
				printf("Token: Type: %s Value: %c\n", token_types_list[tokens[c]->type], tokens[c]->value.is_char);
				break;
			case STR:
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

void tokens_free(TOKEN** tokens, int tokens_count)
{
	while(--tokens_count >= 0)
	{
		if(tokens[tokens_count]->type == STR || tokens[tokens_count]->type == IDN) free(tokens[tokens_count]->value.is_str);
		free(*(tokens+tokens_count));
	}
}
