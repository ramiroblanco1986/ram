#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

int ram_lexer(FILE* file_desc)
{
	char ch;
	char* lexer_buff;

	int tokens_count = 0;
	TOKEN* token = (void*)0;
	TOKEN** tokens = malloc(sizeof(TOKEN*));
	//*tokens = (void*)0;

	do
        {
		ch = fgetc(file_desc);

		if(is_opr(ch))
		{
			printf("es ope: %c\n",ch);
			token = malloc(sizeof(TOKEN));
			token->type = OPR;
			token->value.is_char = ch;
			if(!tokens_push(tokens,token,&tokens_count))
			{
				perror("Error push!\n");
				exit(1);
			}
			continue;
		}
		/*else if(is_str(ch))
		{
			lexer_buff = malloc(sizeof(char)*MAX_STR_LEN);
			lexer_buff[0] = '\0';
			while((ch = fgetc(file_desc)) != EOF && !is_str(ch))
			{
				strncat(lexer_buff, &ch, 1);
			}
			token = malloc(sizeof(TOKEN));
			token->type = STR;
			token->value.is_str = calloc(strlen(lexer_buff)+1, sizeof(char));
			strcpy(token->value.is_str, lexer_buff);
			tokens_push(tokens,token,&tokens_count);
			free(lexer_buff);
			continue;
		}*/
		/*else if(is_idn(ch))
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
			token->value.is_str = calloc(strlen(lexer_buff)+1, sizeof(char));
			printf("i_%s\n", lexer_buff);
			strcpy(token->value.is_str, lexer_buff);
			tokens_push(tokens,token,&tokens_count);
			free(lexer_buff);
		}
		switch(ch)
		{
			case '{':
				token = malloc(sizeof(TOKEN));
				token->type = LCB;
				tokens_push(tokens,token,&tokens_count);
				break;
			case '}':
				token = malloc(sizeof(TOKEN));
				token->type = RCB;
				tokens_push(tokens,token,&tokens_count);
				break;
			case '(':
				printf("__________________________%c\n",ch);
				token = malloc(sizeof(TOKEN));
				token->type = LPR;
				tokens_push(tokens,token,&tokens_count);
				break;
			case ')':
				token = malloc(sizeof(TOKEN));
				token->type = RPR;
				tokens_push(tokens,token,&tokens_count);
				break;
			case ';':
				token = malloc(sizeof(TOKEN));
				token->type = SCN;
				token->value.is_str = malloc(sizeof(char));
				token->value.is_str = '\0';
				tokens_push(tokens,token,&tokens_count);
				break;
		}*/
		
		printf("_%c\n",ch);
        }
	while (ch != EOF);

	//tokens_free(tokens, tokens_count);
	//free(tokens);
	tokens_print(tokens, tokens_count);
	printf("END count: %i\n",tokens_count);

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

int tokens_push(TOKEN** tokens, TOKEN* token, int* tokens_count)
{
	TOKEN* p_tmp;
	printf("_______%li\n", sizeof(TOKEN*)* ((*tokens_count)+1));
	if(!*tokens_count) tokens[*tokens_count] = token;
	else
	{
		printf("XXxXXXSxsxSXSxSXS %i %li\n", *tokens_count, sizeof(TOKEN*)* ((*tokens_count)+1));
		if((p_tmp = realloc(tokens, sizeof(TOKEN*)* ((*tokens_count)+1) )) == NULL) return 0;
		tokens = (TOKEN**)p_tmp;
		tokens[*tokens_count] = token;
	}
	printf(")))))))))))) %i %i %li %p\n", *tokens_count, *tokens_count,sizeof(TOKEN*),tokens[*tokens_count]);
	(*tokens_count)++;
	return 1;
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
	do free(*(tokens+tokens_count));
	while(--tokens_count >= 0);
}
