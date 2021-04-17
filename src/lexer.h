#ifndef _LEXER_H_

#define _LEXER_H_

#define MAX_IDN_LEN 100
#define MAX_STR_LEN 5000
#define DEF_OPERATORS "<>!+-*/%="
#define DEF_IDENTIFIERS "abcdefghijklmnopkrstuvwxyzABCDEFGHIJKLMNOPKRSTUVWXYZ0123456789"

typedef enum
{
	STR,
	LPR,
	RPR,
	IDN,
	OPR,
	LCB,
	RCB,
	SCN
} TOKEN_TYPE;

typedef struct
{
	TOKEN_TYPE type;
	union 
	{
		int is_int;
		float is_float;
		char is_char;
		char* is_str;
		void* is_null;
	} value;
	
} TOKEN;


int ram_lexer(FILE* file_desc);

int is_idn(char ch);
int is_str(char ch);
int is_opr(char ch);

int tokens_count(TOKEN** tokens);
int tokens_push(TOKEN** tokens, TOKEN* token);
void tokens_print(TOKEN** tokens);
void tokens_free(TOKEN** tokens);

#endif //_LEXER_H_

