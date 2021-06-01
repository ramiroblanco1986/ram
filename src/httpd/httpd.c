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
#include <pthread.h>
#include <sys/sysinfo.h>
#include "httpd.h"
#include "request.h"
#include "websocket.h"


int main()
{

	int i;
	char response[200];
	pthread_t threads[MAX_THREADS];
	int threads_c = 0;
	pthread_t ws_threads[MAX_WS_THREADS];
	int wc_threads_c = 0;
	char buffer[BUFFER];
	char raw_req[MAX_REQUEST_SIZE];
	int live = 1;
	int option = 1;
	int recv_c = 0;
	int recv_c_tot = 0;
	int socket_fd, bind_listen_aux = 0;
	int* accept_fd[MAX_THREADS];
	struct sockaddr_in serv_addr, clnt_addr;
	unsigned int sockaddr_in_size = sizeof(struct sockaddr_in);
	REQUEST* request;
	HEADER** headers;
	//REQUEST* request = (REQUEST*) malloc(sizeof(REQUEST));
	//HEADER** headers = (HEADER**) malloc(sizeof(HEADER)*MAX_HEADERS);
	int headers_c;

	printf("This system has %d processors configured and %d processors available.\n", get_nprocs_conf(), get_nprocs());

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

	/*epoll_event.events = EPOLLIN | EPOLLET;
	epoll_event.data.fd = socket_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &epoll_event) == -1) {
		perror("epoll_ctl: listen_sock");
		exit(EXIT_FAILURE);
	}*/

	while(live)
	{

		/*nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}

		for (n = 0; n < nfds; ++n)
		{
			printf("DDDDDDDDD %i %i\n", n, events[n].data.fd);
			if (events[n].data.fd == socket_fd)
			{
				printf("es socket_fd\n");
		*/		if ((accept_fd[threads_c] = accept(socket_fd, (struct sockaddr *)&clnt_addr, &sockaddr_in_size)) == -1)
				{
					fprintf(stderr, "Error en función accept. Código de error %s\n", strerror(accept_fd[threads_c]));
					return -1;
				}
				/* Change the socket into non-blocking state */
				//fcntl(accept_fd, F_SETFL, O_NONBLOCK);
				/*epoll_event.events = EPOLLIN | EPOLLET;
				epoll_event.data.fd = accept_fd;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, accept_fd, &epoll_event) == -1)
				{
					perror("epoll_ctl: accept_sock");
					exit(1);
				}*/
			//}
			//else
			//{
				//printf("es fd: %i\n", events[n].data.fd);
				//GET REQUEST
				recv_c_tot = 0;
				memset(response, 0, 200);
				memset(buffer, 0, BUFFER);
				memset(raw_req, 0, MAX_REQUEST_SIZE);
				while((recv_c=recv(accept_fd[threads_c], buffer, BUFFER, 0)) > 0)
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

				request = (REQUEST*) malloc(sizeof(REQUEST));
				headers = (HEADER**) malloc(sizeof(HEADER)*MAX_HEADERS);
				if(!process_raw_req(raw_req, recv_c_tot, request, headers, &headers_c))
				{
					fprintf(stderr, "Error en función accept. Código de error %s\n", strerror(accept_fd[threads_c]));
					continue;
				}

				printf("METHOD(%s)\n",request->method);
				printf("METHOD_CHARSUM(%i)\n",request->method_charsum);
				printf("URI(%s)\n",request->uri);
				printf("HTTPV(%s)\n",request->httpv);
				printf("HEADERS_C: %i\n", headers_c);


				switch(request->method_charsum)
				{
					case CSUM_GET:

						/* WEBSOCKETS
						 *   1.   An HTTP/1.1 or higher GET request, including a "Request-URI"
							[RFC2616] that should be interpreted as a /resource name/
							defined in Section 3 (or an absolute HTTP/HTTPS URI containing
							the /resource name/).

						   2.   A |Host| header field containing the server's authority.

						   3.   An |Upgrade| header field containing the value "websocket",
							treated as an ASCII case-insensitive value.

						   4.   A |Connection| header field that includes the token "Upgrade",
							treated as an ASCII case-insensitive value.

						   5.   A |Sec-WebSocket-Key| header field with a base64-encoded (see
							Section 4 of [RFC4648]) value that, when decoded, is 16 bytes in
							length.

						   6.   A |Sec-WebSocket-Version| header field, with a value of 13.
						 * */

						strcpy(response, "HTTP/1.1 200 OK\r\n\r\n<h1>GET REQUEST</h1>");
						strcat(response, request->method);
						break;
					case CSUM_POST:
						strcpy(response, "HTTP/1.1 200 OK\r\n\r\n<h1>GET POST</h1>");
						strcat(response, request->method);
						break;
					default:
						strcpy(response, "HTTP/1.1 503 OK\r\n\r\n<h1>UNKNOWN</h1>");
						strcpy(response, request->method);
						break;

				}


				/*end request*/
				write (accept_fd[threads_c], response , strlen(response));
				if(headers_c) free_headers(headers, headers_c);
				free(headers); headers = NULL;
				free(request); request = NULL;
				close(accept_fd[threads_c]);
				continue;

				//strcpy(response, "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Protocol: binary\r\nSec-WebSocket-Accept: ");
				//strcpy(response, "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ");
				//strcat(response, process_raw_req(raw_req, recv_c_tot, request, headers));
				//strcat(response, "\r\n\r\n");

				//printf("____\n%s", response);
				//write (accept_fd, response , strlen(response));
				printf("__^_\n");
				//sleep(10);
				//close(events[n].data.fd);

				//strcpy(response, "129");
				//write (accept_fd, response , strlen(response));
				//write (accept_fd, raw_req, recv_c_tot);
				//recv_c=recv(accept_fd, buffer, 100, 0);
				//printf("%s\n",buffer);

				pthread_create(&threads[threads_c], NULL, read_websocket, accept_fd[threads_c]);
				threads_c++;
				//read_websocket(events[n].data.fd);

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
				//printf("EXIT\n");
				//close(accept_fd);
		//	}
		//}

		//printf  ("Server:  conexión desde:  %s", inet_ntoa(clnt_addr.sin_addr));

		//printf(". End.\n");
		//sleep(1);
	//}

	//if(close(epoll_fd))
	//{
	//	fprintf(stderr, "Failed to close epoll file descriptor\n");
	//	return 1;
	}

}

void free_headers(HEADER** headers, int headers_c)
{
	while(--headers_c > -1)
	{
		free(headers[headers_c]->name);
		free(headers[headers_c]->value);
		free(headers[headers_c]);
	}

}
