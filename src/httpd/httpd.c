#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <fcntl.h>
#include <limits.h>
#include "httpd.h"
#include "request.h"


int main()
{

	int i;
	char response[200];
	char buffer[BUFFER];
	char raw_req[MAX_REQUEST_SIZE];
	int live = 1;
	int option = 1;
	int recv_c = 0;
	int recv_c_tot = 0;
	int socket_fd, accept_fd, bind_listen_aux = 0;
	unsigned int bit_curr;
	unsigned int pay_length;
	unsigned char MASK[4];
	unsigned int FIN;
	unsigned int max;
	struct sockaddr_in serv_addr, clnt_addr;
	unsigned int sockaddr_in_size = sizeof(struct sockaddr_in);
	REQUEST* request = (REQUEST*) malloc(sizeof(REQUEST));
	HEADER** headers = (HEADER**) malloc(sizeof(HEADER)*MAX_HEADERS);


	//struct epoll_event epoll_event, events[MAX_EVENTS];
	//int epoll_fd = epoll_create1(0);
	//int n,nfds;

	//if(epoll_fd == -1)
	//{
	//	fprintf(stderr, "Failed to create epoll file descriptor\n");
	//	return -1;
	//}

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		fprintf(stderr, "Error en función socket. Código de error %s\n", strerror(socket_fd));
		return -1;
	}
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(81);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));


	if ((bind_listen_aux = bind(socket_fd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr))) == -1)
	{
		fprintf(stderr, "Error en función bind. Código de error %s\n", strerror(bind_listen_aux));
		return -1;
	}
	if ((bind_listen_aux = listen(socket_fd, BACKLOG)) == -1)
	{
		fprintf(stderr, "Error en función listen. Código de error %s\n", strerror(bind_listen_aux));
		return -1;
	}

	//epoll_event.events = EPOLLIN;
	//epoll_event.data.fd = socket_fd;
	//if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &epoll_event) == -1) {
	//	perror("epoll_ctl: listen_sock");
	//	exit(EXIT_FAILURE);
	//}

	while(live)
	{

		//nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		//if (nfds == -1) {
		//	perror("epoll_wait");
		//	exit(EXIT_FAILURE);
		//}

		//for (n = 0; n < nfds; ++n)
		//{
		//	if (events[n].data.fd == socket_fd)
		//	{
				if ((accept_fd = accept(socket_fd, (struct sockaddr *)&clnt_addr, &sockaddr_in_size)) == -1)
				{
					fprintf(stderr, "Error en función accept. Código de error %s\n", strerror(accept_fd));
					return -1;
				}
				//fcntl(accept_fd, F_SETFL, O_NONBLOCK); /* Change the socket into non-blocking state 
		//		epoll_event.events = EPOLLIN | EPOLLET;
		//		epoll_event.data.fd = accept_fd;
		//		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, accept_fd, &epoll_event) == -1)
		//		{
		//			perror("epoll_ctl: accept_sock");
		//			exit(1);
		//		}
		//	}
		//	else
		//	{
				//GET REQUEST
				recv_c_tot = 0;
				memset(buffer, 0, BUFFER);
				memset(raw_req, 0, MAX_REQUEST_SIZE);
				while((recv_c=recv(accept_fd, buffer, BUFFER, 0)) > 0)
				{
					for(i=0;i<recv_c;i++)
					{	
						raw_req[recv_c_tot] = buffer[i];
						recv_c_tot++;
					}
					if(recv_c_tot > 5) if(raw_req[recv_c_tot-1] == LF 
							&& raw_req[recv_c_tot-2] == CR
							&& raw_req[recv_c_tot-3] == LF
							&& raw_req[recv_c_tot-4] == CR
							) break;
				}
				raw_req[recv_c_tot] = '\0';

				//strcpy(response, "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Protocol: binary\r\nSec-WebSocket-Accept: ");
				strcpy(response, "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ");
				strcat(response, process_raw_req(raw_req, recv_c_tot, request, headers));
				strcat(response, "\r\n\r\n");

				//printf("____\n%s", response);
				//write (accept_fd, response , strlen(response));
				write (accept_fd, response , strlen(response));
				printf("__^_\n");
				//strcpy(response, "129");
				//write (accept_fd, response , strlen(response));
				//write (accept_fd, raw_req, recv_c_tot);
				//recv_c=recv(accept_fd, buffer, 100, 0);
				//printf("%s\n",buffer);
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
					printf(">%i<\n",recv_c);


				//while(1)
				//{
				//	write (accept_fd, request->uri, sizeof(request->uri));
				//	sleep(1);
				//}
				//write (accept_fd, request->uri, sizeof(request->uri));
				//if (write (accept_fd, raw_req, recv_c_tot) == -1)
				//{
				//	printf("Error escribiendo mensaje en socket");
				//	exit (1);
				//}
				/*if (write (events[n].data.fd, "chau" , sizeof(message)) == -1)
                                {
                                        perror("Error escribiendo mensaje en socket");
                                        //exit (1);
                                }*/
				printf("EXIT\n");
				close(accept_fd);
		//	}
		//}

		//printf  ("Server:  conexión desde:  %s", inet_ntoa(clnt_addr.sin_addr));

		//printf(". End.\n");
		//sleep(1);
	}

	/*if(close(epoll_fd))
	{
		fprintf(stderr, "Failed to close epoll file descriptor\n");
		return 1;
	}*/

}

