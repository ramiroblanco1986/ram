#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include "websocket.h"

int read_websocket(int accept_fd)
{
	int i;
	int recv_c = 0;
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
		OPCODE = bit_curr&0xF ? 1 : 0;
		printf("FIN: %i OPCODE: %i\n", FIN, OPCODE);
		switch (OPCODE) 
		{
			case OPC_TEXT:
				printf("TEXT\n");
				//RSRV123
				recv_c=recv(accept_fd, &bit_curr, 1, 0);
				//MASK = bit_curr&0x80 ? 1 : 0;
				pay_length = bit_curr&0x7F;
				printf("MASK %i Pay Length: %i\n", bit_curr&0x80, pay_length);
				if(pay_length < 126)
				{
					printf("Pay_length: %i\n", pay_length);
					for(i=0;i<4;i++) {
						recv_c=recv(accept_fd, &bit_curr, 1, 0);
						//printf("_____ %i \n", bit_curr);
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
					printf("\n>");
					for (i = 0; i < pay_length; i++)
					{
						//DECODED[i] = ENCODED[i] ^ MASK[i % 4];
						printf("%c", buffer[i] ^ MASK[ i % 4]);
					}
					printf("<\n");
				}
				break; //OPC_TEXT 
		}
		while(1)
		{
			i = 129;
			write (accept_fd, &i, 1);
			i = 4;
			write (accept_fd, &i, 1);
			i = 104;
			write (accept_fd, &i, 1);
			i = 111;
			write (accept_fd, &i, 1);
			i = 108;
			write (accept_fd, &i, 1);
			i = 97;
			write (accept_fd, &i, 1);
			sleep(1);
		}
	}
}
