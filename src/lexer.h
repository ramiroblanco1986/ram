#ifndef LEXER_H

#define LEXER_H 

typedef enum
{
	STR,
	EQL,
	LPR,
	RPR,
	IDN,
	OPR,
	LCB,
	RCB,
	SCN
} TOKEN_TYPE;

const char* token_types_list[] = { "STR", "EQL", "LPR", "RPR", "IDN", "OPR", "LCB", "RCB", "SCN" };

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

#endif
