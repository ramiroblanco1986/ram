#include <stdio.h>
#include "websocket.h"


int read_websocket(int accept_fd)
{

	int i;
	int recv_c = 0;
	int accept_fd;
	char buffer[BUFFER];
	unsigned int bit_curr;
	unsigned int pay_length;
	unsigned char MASK[4];
	unsigned int FIN;
	unsigned int OPCODE;
	unsigned int max;

	memset(buffer, 0, BUFFER);
	while(recv_c=recv(accept_fd, &bit_curr, 1, 0))
	{

		max = 1<<(1*CHAR_BIT-1);
		printf("\n");
		for(i=1;i<=1*CHAR_BIT;i++)
		{
			printf("%i", (bit_curr&max ? 1 : 0));
			if(i % 8 == 0) printf(" ");
			max >>= 1;
		}
		//FIN
		FIN = bit_curr&0x80 ? 1 : 0;
		printf("FIN: %i\n", FIN);
		//RSRV123
		recv_c=recv(accept_fd, &bit_curr, 1, 0);
		//MASK = bit_curr&0x80 ? 1 : 0;
		//printf("MASK %i\n", MASK);
		pay_length = bit_curr&0x7F;
		if(pay_length < 126)
		{
			printf("Pay_length: %i\n", pay_length);
			for(i=0;i<4;i++) {
				recv_c=recv(accept_fd, &bit_curr, 1, 0);
				printf("_____ %i \n", bit_curr);
				MASK[i] = (char)bit_curr;
			}
			//printf("MASK %s\n", MASK);
			max = 1<<(4*CHAR_BIT-1);
			for(i=1;i<=4*CHAR_BIT;i++)
			{
				printf("%i", (bit_curr&max ? 1 : 0));
				if(i % 8 == 0) printf(" ");
				max >>= 1;
			}
			memset(buffer, 0, BUFFER);
			recv_c=recv(accept_fd, &buffer, pay_length, 0);
			printf("\n%s\n",buffer);
			printf(">");
			for (i = 0; i < pay_length; i++)
			{
				//DECODED[i] = ENCODED[i] ^ MASK[i % 4];
				printf("%c", buffer[i] ^ MASK[ i % 4]);
			}
			printf("<\n");
		}
		//opcode
		//recv_c=recv(accept_fd, &length, 1, 0);
		//printf(">>>> %i \n\n", recv_c);
		//if(length&0x80) printf("MASK True\n");
		//printf(">>>> %u \n\n", length);
		//for(i=0;i<32;i++)
		//printf(">>>> %u \n\n", length);
		//printf(">>>> %li \n\n", sizeof(unsigned int));
		//for(aux_c=)
		//printf("_\n");
	}
}

