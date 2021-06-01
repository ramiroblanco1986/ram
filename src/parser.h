#ifndef _PARSER_H_

#define _PARSER_H_

typedef struct NODE NODE;

struct NODE
{
	TOKEN* token;
	NODE* left;
	NODE* right;

};


int parse(TOKEN** tokens, int tokens_count);

#endif //_PARSER_H_

