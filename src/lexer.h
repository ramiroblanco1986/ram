#ifndef LEXER_H

#define LEXER_H

#endif

typedef enum
{
	STR,
	EQL,
	LPR,
	RPR,
	IDN,
	OPR
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

