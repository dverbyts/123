#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "DNS.h"

int main(int argc, char **argv)
{

	struct		sockaddr_in server;
	struct		sockaddr_in adr;
	struct		sockaddr_in receive;
	int			socket_fd;
	char		buf[576];
	char		buf2[576];
	ssize_t		recive;
	ssize_t		sent;
	socklen_t	from_len;

	if (argc == 1)
		strcpy(buf, "hello world!");
	else
		strcpy(buf, argv[1]);

	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_fd < 0)
	{
		perror("socket_fd");
		exit(EXIT_FAILURE);
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(1234);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	memset(&adr, 0, sizeof(adr));
	adr.sin_family = AF_INET;
	adr.sin_port = htons(1233);
	adr.sin_addr.s_addr = htonl(INADDR_ANY);


	memset(&receive, 0, sizeof(receive));
	receive.sin_family = AF_INET;
	from_len = sizeof(receive);

	if ((bind(socket_fd, (struct sockaddr *)&server, sizeof(server))) < 0)
	{
		perror("bind");
		close(socket_fd);
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		printf("port_server: %i addr_server: %i\n", server.sin_port, server.sin_addr);
		printf("port_adr: %i addr_adr: %i\n", adr.sin_port, adr.sin_addr);
		printf("port_receive: %i addr_receive: %i\n\n", receive.sin_port, receive.sin_addr);
		if ((sent = sendto(socket_fd, buf, strlen(buf), 0, (struct sockaddr*)&adr, sizeof(adr))) < 0)
		{
			perror("sendto");
			close(socket_fd);
			exit(EXIT_FAILURE);
		}		
		printf("sent: %i, \"%s\", struct size %i\n", sent, buf, sizeof(adr));
		printf("port_server: %i addr_server: %i\n", server.sin_port, server.sin_addr);
		printf("port_adr: %i addr_adr: %i\n", adr.sin_port, adr.sin_addr);
		printf("port_receive: %i addr_receive: %i\n\n", receive.sin_port, receive.sin_addr);
		if ((recive = recvfrom(socket_fd, buf2, sizeof(buf2), 0, (struct sockaddr *)&receive, &from_len)) < 0)
		{
			perror("receivefrom");
			close(socket_fd);
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("receive: %i\n", recive);
			printf("port_server: %i addr_server: %i\n", server.sin_port, server.sin_addr);
			printf("port_adr: %i addr_adr: %i\n", adr.sin_port, adr.sin_addr);
			printf("port_receive: %i addr_receive: %i\n\n", receive.sin_port, receive.sin_addr);
			printf("datagram: %s\n", buf2);
			close(socket_fd);
			return (1);
		}
		// for (int i = 0; i < 576; ++i)
		// {
		// 	printf("%c", buf[i]);
		// 	printf("\n");
		// 	z = 0;
		// }
//		// 576 байт гарантировано дойдет
	}
	return (1);
//
//	close(sock); /* close the socket */
//	return 0;
}