#ifndef _HTTPD_H_

#define _HTTPD_H_

#define BUFFER 4

#define BACKLOG 10
#define MAX_EVENTS 2 

#define MAX_HEADER_SIZE 8000000 // 8K 

#define MAX_HEADERS 80 // 80headers 

#define LF 10
#define CR 13

typedef struct
{
	char* name;
	char* value;
} HEADER;


#endif //_HTTPD_H_

