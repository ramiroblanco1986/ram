#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <fcntl.h>
#include "httpd.h"


int main()
{

	char message[] = "HTTP/1.0 200 OK\r\n\r\n<h1>OK Response</h1>";
	char buffer[256];
	char header[MAX_HEADER_SIZE];
	int header_c = 0;
	int live = 1;
	int option = 1;
	int socket_fd, accept_fd, bind_listen_aux = 0;
	struct sockaddr_in serv_addr, clnt_addr;
	unsigned int sockaddr_in_size = sizeof(struct sockaddr_in);

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
	serv_addr.sin_port = htons(80);
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
				while( (header_c=read(accept_fd, buffer, 256)) > 0)
				{
					printf("%s",buffer);
					if(buffer[--header_c] == '\n') break;
				}
				if (write (accept_fd, message , sizeof(message)) == -1)
				{
					perror("Error escribiendo mensaje en socket");
					exit (1);
				}
				/*if (write (events[n].data.fd, "chau" , sizeof(message)) == -1)
                                {
                                        perror("Error escribiendo mensaje en socket");
                                        //exit (1);
                                }*/
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

