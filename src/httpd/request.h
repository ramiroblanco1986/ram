#ifndef _REQUEST_H_

#define _REQUEST_H_

#define MAX_REQUEST_SIZE 8000 // 8K 
#define METHOD_SIZE 10
#define URI_SIZE 2000
#define HTTPV_SIZE 20
#define FRAME_SIZE 4

#define WSUUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

//METHODS
#define GET 224

typedef struct
{
	int method;
        char uri[URI_SIZE];
        char httpv[HTTPV_SIZE];
} REQUEST;

char* process_raw_req(char* raw_req, int recv_c_tot, REQUEST* request, HEADER** headers);

//HEADERS
#define SECWEBSOCKETKEY "sec-websocket-key"

//char headers_names[5][] =
//	{
//		"host",
//		"connection",
//		"sec-websocket-key",
//		"sec-webSocket-version",
//		"upgrade",
//	}

#endif //_REQUEST_H_

