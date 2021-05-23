#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "httpd.h"
#include "request.h"
#include "base64.h"


char* process_raw_req(char* raw_req, int recv_c_tot, REQUEST* request, HEADER** headers)
{
	unsigned char hash[SHA_DIGEST_LENGTH];
	unsigned char hash_ori[100];
	unsigned char wsuuid[] = WSUUID;
	int ch = 0, aux_c = 0, aux_h = 0, headers_c = 0;
	request->method = 0;

	/* The Request-Line begins with a method token, followed by the
	   Request-URI and the protocol version, and ending with CRLF. The
	   elements are separated by SP characters. No CR or LF is allowed
	   except in the final CRLF sequence.
	   Method SP Request-URI SP HTTP-Version CRLF */

	//Method
	while(raw_req[ch] != SP) request->method += (int)raw_req[ch++];

	ch++; //skip SP

	//Uri
	while(raw_req[ch] != SP) request->uri[aux_c++] = raw_req[ch++];

	request->uri[ch] = '\0';

	ch++; //skip SP

	//HTTP Version
	aux_c = 0;
	while(raw_req[ch] != CR) request->httpv[aux_c++] = raw_req[ch++];

	request->httpv[ch] = '\0';

	if(raw_req[++ch] != LF) return -1; //TODO

	printf("METHOD(%i)\n",request->method);
	printf("URI(%s)\n",request->uri);
	printf("HTTPV(%s)\n",request->httpv);

	ch++; //skip LF

	//Headers
	/* Header fields are colon-separated key-value pairs in clear-text string format,
	   terminated by a carriage return (CR) and line feed (LF) character sequence. */

	while(1)
	{
		headers[headers_c] = (HEADER*) malloc(sizeof(HEADER));
		headers[headers_c]->name = (char*) malloc(sizeof(char)*MAX_HEADER_NAME_SIZE);
		headers[headers_c]->value = (char*) malloc(sizeof(char)*MAX_HEADER_VALUE_SIZE);
		//Header Name
		aux_c = 0;
		while(raw_req[ch] != SC && aux_c < MAX_HEADER_NAME_SIZE) headers[headers_c]->name[aux_c++] = raw_req[ch++];
		headers[headers_c]->name[aux_c] = '\0';
		ch++; //skip SC
		aux_c = 0;
		while(raw_req[ch] != CR)
		{
			if(raw_req[ch] == SP)
			{
				ch++;
				continue;
			}
			headers[headers_c]->value[aux_c++] = raw_req[ch++];
		}
		if(raw_req[++ch] != LF) return -1; //TODO
		headers[headers_c]->value[aux_c] = '\0';
		headers_c++;
		if(raw_req[ch+1] == CR && raw_req[ch+2] == LF) break;
		ch++; //skip LF
	}

	printf("HEADERS_C: %i\n", headers_c);
	headers_c--;
	for(;headers_c>0;headers_c--)
	{
		aux_c=0;
		//aux_h=0;
		//ToLower
		while(headers[headers_c]->name[aux_c] != '\0') { 
			if( headers[headers_c]->name[aux_c] < 91 && headers[headers_c]->name[aux_c] > 64) headers[headers_c]->name[aux_c] += 32;
			//printf("_%i_%c_", (int)headers[headers_c]->name[aux_c], headers[headers_c]->name[aux_c]);
			//aux_h+=(int)headers[headers_c]->name[aux_c];
			aux_c++;
		}
		if(aux_c == 17)
		if(headers[headers_c]->name[0] == 115 &&
				headers[headers_c]->name[1] == 101 &&
				headers[headers_c]->name[2] == 99 &&
				headers[headers_c]->name[3] == 45 &&
				headers[headers_c]->name[4] == 119 &&
				headers[headers_c]->name[5] == 101 &&
				headers[headers_c]->name[6] == 98 &&
				headers[headers_c]->name[7] == 115 &&
				headers[headers_c]->name[8] == 111 &&
				headers[headers_c]->name[9] == 99 &&
				headers[headers_c]->name[10] == 107 &&
				headers[headers_c]->name[11] == 101 &&
				headers[headers_c]->name[12] == 116 &&
				headers[headers_c]->name[13] == 45 &&
				headers[headers_c]->name[14] == 107 &&
				headers[headers_c]->name[15] == 101 &&
				headers[headers_c]->name[16] ==  121)
		{
			printf("%s: %s\n", headers[headers_c]->name, headers[headers_c]->value);
			aux_c = 0;
			while(headers[headers_c]->value[aux_c] != '\0')
			{
				hash_ori[aux_c] = headers[headers_c]->value[aux_c];
				aux_c++;
			}
			aux_h = 0;
			while(wsuuid[aux_h] != '\0')
			{
				hash_ori[aux_c] = wsuuid[aux_h];
				aux_c++;
				aux_h++;
			}
			hash_ori[aux_c] = '\0';

			//for(aux_c=0;headers[headers_c]->value[aux_c] != '\0';aux_c++) hash_ori[aux_c] = headers[headers_c]->value[aux_c];

			printf(">>>>>>>>> %s\n",hash_ori);
			//printf("%s\n",SHA1(hash_ori, strlen(hash_ori), hash));
			SHA1(hash_ori, strlen(hash_ori), hash);
			//printf("%s\n", hash);
			//printf("%s\n",b64_encode(hash, sizeof(hash)));
		}
		//printf("%s(%i) => ",headers[headers_c]->name, aux_h);

	}
	return b64_encode(hash, strlen(hash));
}
