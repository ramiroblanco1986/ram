#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "httpd.h"
#include "request.h"
#include "base64.h"


int process_raw_req(char* raw_req, int recv_c_tot, REQUEST* request, HEADER** headers, int* headers_c)
{
	//unsigned char hash[SHA_DIGEST_LENGTH];
	//unsigned char hash_ori[100];
	//unsigned char wsuuid[] = WSUUID;
	int ch = 0, aux_c = 0, aux_h = 0;

	/* The Request-Line begins with a method token, followed by the
	   Request-URI and the protocol version, and ending with CRLF. The
	   elements are separated by SP characters. No CR or LF is allowed
	   except in the final CRLF sequence.
	   Method SP Request-URI SP HTTP-Version CRLF */

	//Method
	request->method_charsum = 0;
	while(raw_req[ch] != SP && aux_c < METHOD_SIZE)
	{
		request->method_charsum += (int) raw_req[ch];
		request->method[aux_c++] = raw_req[ch++];
	}
	request->method[aux_c] = '\0';

	ch++; //skip SP

	//Uri
	aux_c = 0;
	while(raw_req[ch] != SP && aux_c < URI_SIZE) request->uri[aux_c++] = raw_req[ch++];
	request->uri[aux_c] = '\0';

	request->uri[ch] = '\0';

	ch++; //skip SP

	//HTTP Version
	aux_c = 0;
	while(raw_req[ch] != CR && aux_c < HTTPV_SIZE) request->httpv[aux_c++] = raw_req[ch++];
	request->httpv[aux_c] = '\0';

	request->httpv[ch] = '\0';

	if(raw_req[++ch] != LF) return -1; //TODO

	ch++; //skip LF

	//Headers
	/* Header fields are colon-separated key-value pairs in clear-text string format,
	   terminated by a carriage return (CR) and line feed (LF) character sequence. */

	aux_h = 0;
	while(1)
	{
		headers[aux_h] = (HEADER*) malloc(sizeof(HEADER));
		headers[aux_h]->name = (char*) malloc(sizeof(char)*MAX_HEADER_NAME_SIZE);
		headers[aux_h]->value = (char*) malloc(sizeof(char)*MAX_HEADER_VALUE_SIZE);
		//Header Name
		aux_c = 0;
		while(raw_req[ch] != SC && aux_c < MAX_HEADER_NAME_SIZE) headers[aux_h]->name[aux_c++] = raw_req[ch++];
		headers[aux_h]->name[aux_c] = '\0';
		ch++; /* skip SC */
		aux_c = 0;
		while(raw_req[ch] != CR)
		{
			if(raw_req[ch] == SP)
			{
				ch++;
				continue;
			}
			headers[aux_h]->value[aux_c++] = raw_req[ch++];
		}
		if(raw_req[++ch] != LF) return -1; //TODO
		headers[aux_h]->value[aux_c] = '\0';
		aux_h++;
		if(raw_req[ch+1] == CR && raw_req[ch+2] == LF) break;
		ch++; /* skip LF */
	}

	*headers_c = aux_h; /* store headers count */

	aux_h--;
	for(;aux_h>0;aux_h--)
	{
		aux_c=0;
		//ToLower
		while(headers[aux_h]->name[aux_c] != '\0') { 
			if( headers[aux_h]->name[aux_c] < 91 && headers[aux_h]->name[aux_c] > 64) headers[aux_h]->name[aux_c] += 32;
			aux_c++;
		}
		


		/*if(aux_c == 17)
		if(headers[aux_h]->name[0] == 115 &&
				headers[aux_h]->name[1] == 101 &&
				headers[aux_h]->name[2] == 99 &&
				headers[aux_h]->name[3] == 45 &&
				headers[aux_h]->name[4] == 119 &&
				headers[aux_h]->name[5] == 101 &&
				headers[aux_h]->name[6] == 98 &&
				headers[aux_h]->name[7] == 115 &&
				headers[aux_h]->name[8] == 111 &&
				headers[aux_h]->name[9] == 99 &&
				headers[aux_h]->name[10] == 107 &&
				headers[aux_h]->name[11] == 101 &&
				headers[aux_h]->name[12] == 116 &&
				headers[aux_h]->name[13] == 45 &&
				headers[aux_h]->name[14] == 107 &&
				headers[aux_h]->name[15] == 101 &&
				headers[aux_h]->name[16] ==  121)
		{
			printf("%s: %s\n", headers[aux_h]->name, headers[aux_h]->value);
			aux_c = 0;
			while(headers[aux_h]->value[aux_c] != '\0')
			{
				hash_ori[aux_c] = headers[aux_h]->value[aux_c];
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

			//for(aux_c=0;headers[aux_h]->value[aux_c] != '\0';aux_c++) hash_ori[aux_c] = headers[aux_h]->value[aux_c];

			printf(">>>>>>>>> %s\n",hash_ori);
			//printf("%s\n",SHA1(hash_ori, strlen(hash_ori), hash));
			SHA1(hash_ori, strlen(hash_ori), hash);
			//printf("%s\n", hash);
			//printf("%s\n",b64_encode(hash, sizeof(hash)));
		}
		//printf("%s(%i) => ",headers[aux_h]->name, aux_h);
		*/

	}
	//return b64_encode(hash, strlen(hash));
	return 1;
}
