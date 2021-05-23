#ifndef _HTTPD_H_

#define _HTTPD_H_

#define BUFFER 4

#define BACKLOG 10
#define MAX_EVENTS 2 

#define MAX_HEADERS 80 // 80headers 

#define MAX_HEADER_NAME_SIZE 2000
#define MAX_HEADER_VALUE_SIZE 6000

#define LF 10
#define CR 13
#define SP 32
#define SC 58

typedef struct
{
	char* name;
	char* value;
} HEADER;


#endif //_HTTPD_H_

