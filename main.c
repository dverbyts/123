# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
#include "DNS.h"

t_config	*g_c;

void		parse_config_info(int fd);

int		in_list(char *buf)
{
	t_blc_list *tmp;

	tmp = g_c->black_list;
	while (tmp != NULL)
	{
		if (strcmp(tmp->blc_donain, buf))
			return (1);
		tmp = tmp->next_domain;
	}
	return (0);
}

int		main(int argc, char **argv)
{
	int			fd;
	struct		sockaddr_in server;
	struct		sockaddr_in server_new;
	struct		sockaddr_in client;
	struct		sockaddr_in dns_adr;
	struct		sockaddr_in client_adr;
	int			socket_fd;
	int			socket_fd_new;
	char		buf[576];
	char		buf2[576];
	ssize_t		receive;
	ssize_t		sent;
	socklen_t	from_len;

	if (argc != 2) // if !config_file, print usage && exit
	{
		printf("Usage: ./dns config_file\n");
		return (0);
	}

	if (!(g_c = (t_config *)malloc(sizeof(t_config)))) //malloc memory for global variable
		return (0); //exit program, if memory not allocated
	g_c->black_domain_answer = NULL;
	g_c->black_list = NULL;
	fd = open(argv[1], O_RDONLY); // take fd for read config file
	parse_config_info(fd); // read config info
	close(fd); // close fd

	strcpy(buf2, "message receive!!!!!!!!!!!!!!!!!!!!!!!!!!");

	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_fd < 0)
	{
		perror("socket_fd");
		exit(EXIT_FAILURE);
	}
	memset(&server, 0, sizeof server);
	server.sin_family = AF_INET;
	server.sin_port = htons(1233);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	memset(&dns_adr, 0, sizeof dns_adr);
	dns_adr.sin_family = AF_INET;
	dns_adr.sin_port = htons(PORT);
	dns_adr.sin_addr.s_addr = inet_addr(g_c->hi_ip);

	memset(&client, 0, sizeof client);
	client.sin_family = AF_INET;
	from_len = sizeof(client);

	if ((bind(socket_fd, (struct sockaddr *)&server, sizeof(server))) < 0)
	{
		perror("bind");
		close(socket_fd);
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		// printf("port_server: %i\naddr_server: %i\n", server.sin_port, server.sin_addr);
		// printf("port_client: %i\naddr_client: %i\n\n", client.sin_port, client.sin_addr);
		receive = recvfrom(socket_fd, buf, sizeof(buf), 0, (struct sockaddr *)&client, &from_len);
		if (receive > 0 && in_list(buf))
		{
			if ((sent = sendto(socket_fd, g_c->black_domain_answer, strlen(g_c->black_domain_answer), 0, (struct sockaddr*)&client, sizeof(client))) < 0)
				perror("sendto");
		}
		else if (receive > 0)
		{
			memset(&client_adr, 0, sizeof(client_adr));
			client_adr.sin_family = AF_INET;
			client_adr.sin_port = client.sin_port;
			client_adr.sin_addr.s_addr = client.sin_addr.s_addr;
			memset(&server_new, 0, sizeof(server));
			server_new.sin_family = AF_INET;
			server_new.sin_port = htons(0);
			server_new.sin_addr.s_addr = htonl(INADDR_ANY);
			switch (fork())
			{
				case -1:
					perror("fork");
					break;

				case 0:
					break;

				default:
					socket_fd_new = socket(AF_INET, SOCK_DGRAM, 0);
					if (socket_fd < 0)
					{
						perror("socket_fd");
						_exit(EXIT_FAILURE);
					}
					if ((bind(socket_fd_new, (struct sockaddr *)&server_new, sizeof(server_new))) < 0)
					{
						perror("bind");
						close(socket_fd_new);
						_exit(EXIT_FAILURE);
					}
					//buf = datagram_adr(buf, server_new);
					if ((sent = sendto(socket_fd_new, buf, strlen(buf), 0, (struct sockaddr*)&dns_adr, sizeof(dns_adr))) < 0)
					{
						perror("sendto");
						close(socket_fd_new);
						_exit(EXIT_FAILURE);
					}
					int i = 0;
					while (i++ < TIME_OUT)
					{
						if ((receive = recvfrom(socket_fd_new, buf2, sizeof(buf2), 0, NULL, NULL)) < 0)
						{
							perror("receivefrom");
							close(socket_fd);
							_exit(EXIT_FAILURE);
						}
						//buf2 = datagram_adr(buf2, server);
						if ((sent = sendto(socket_fd, buf2, strlen(buf2), 0, (struct sockaddr*)&client_adr, sizeof(client_adr))) < 0)
						{
							perror("sendto");
							close(socket_fd_new);
							_exit(EXIT_FAILURE);
						}
						else
							i = TIME_OUT;
						if (i == TIME_OUT)
						{
							close(socket_fd_new);
							_exit(EXIT_FAILURE);
						}
					}
					_exit(1);
			}
		}
		// printf("receive: %i\n", receive);
		// printf("port_server: %i addr_server: %i\n", server.sin_port, server.sin_addr);
		// printf("port_client: %i addr_client: %i\n\n", client.sin_port, client.sin_addr);
		// printf("datagram: %s\n", buf);

		
		// //memset(&client.sin_zero, 0, sizeof(client.sin_zero));
		// if ((sent = sendto(socket_fd, buf2, strlen(buf2), 0, (struct sockaddr *)&client, sizeof(client))) < 0)
		// {
		// 	perror("sendto");
		// 	close(socket_fd);
		// 	exit(EXIT_FAILURE);
		// }
		// else
		// {
		// 	printf("sent: %i, \"%s\"\n", sent, buf2);
		// 	printf("port_server: %i addr_server: %i\n", server.sin_port, server.sin_addr);
		// 	printf("port_client: %i addr_client: %i\n\n", client.sin_port, client.sin_addr);
		// 	close(socket_fd);
		// 	return (1);
		// }
		// /* 576 байт гарантировано дойдет */
	}
	return (1);
}