#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include "httpd.h"


int main()
{

	char message[] = "Hola hola hola";
	int live = 1;
	int socket_fd, accept_fd, bind_listen_aux = 0;
	struct sockaddr_in serv_addr, clnt_addr;
	unsigned int sockaddr_in_size = sizeof(struct sockaddr_in);

	struct epoll_event event;
	int epoll_fd = epoll_create1(0);

	if(epoll_fd == -1)
	{
		fprintf(stderr, "Failed to create epoll file descriptor\n");
		return -1;
	}



	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		fprintf(stderr, "Error en función socket. Código de error %s\n", strerror(socket_fd));
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8080);
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

	while(live)
	{
		if ((accept_fd = accept(socket_fd, (struct sockaddr *)&clnt_addr, &sockaddr_in_size)) == -1)
		{
			fprintf(stderr, "Error en función accept. Código de error %s\n", strerror(accept_fd));
			return -1;
		}

		printf  ("Server:  conexión desde:  %s", inet_ntoa(clnt_addr.sin_addr));

		if (write (accept_fd, message , sizeof(message)) == -1)
		{
			perror("Error escribiendo mensaje en socket");
			exit (1);
		}
		printf(". End.\n");
		sleep(1);
	}

}

