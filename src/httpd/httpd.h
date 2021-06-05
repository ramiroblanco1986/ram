#ifndef _HTTPD_H_

#define _HTTPD_H_

#define BUFFER 4

#define BACKLOG 10
#define MAX_EVENTS  100
#define MAX_THREADS 5 
#define MAX_WS_THREADS 5 

#define MAX_HEADERS 80 // 80headers 

#define MAX_HEADER_NAME_SIZE 2000
#define MAX_HEADER_VALUE_SIZE 6000

#define LF 10
#define CR 13
#define SP 32
#define SC 58

#define CSUM_GET 224
#define CSUM_POST 326
#define CSUM_HEAD 274
#define CSUM_PUT 249
#define CSUM_DELETE 435
#define CSUM_CONNECT 522
#define CSUM_TRACE 367
#define CSUM_PATCH 368

typedef struct
{
	char* name;
	char* value;
} HEADER;

void free_headers(HEADER** headers, int headers_c);

#endif //_HTTPD_H_

