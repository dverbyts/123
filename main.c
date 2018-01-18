#include "DNS.h"

t_config	*g_c;

void		parse_config_info(int fd);

int		main(int argc, char **argv)
{
	int			fd = -1;
	struct		sockaddr_in in;
	struct		sockaddr_in to;
	int			socket_in_fd;
	int			socket_to_fd;
	void		*buf;
	ssize_t		recsize;
	socklen_t	*from_len = NULL;

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

	socket_in_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_in_fd < 0)
	{
		perror("socket_in_fd");
		exit(1);
	}
	in.sin_family = AF_INET;
	in.sin_port = htons(PORT);
	in.sin_addr.s_addr = htonl(inet_addr(my_ip));

	if ((bind(socket_in_fd, (struct sockaddr *) &in, sizeof(struct sockaddr))) < 0)
	{
		perror("bind");
		exit(2);
	}

	while(1)
	{
		//printf( "recv test....\n" );
		recsize = recvfrom(socket_in_fd, (void *)buf, 100, 0, (struct sockaddr *)&in, from_len);
		if ( recsize < 0 )
			fprintf( stderr, "Ошибка %s\n", strerror( errno ) );
		//printf("recsize: %d\n ", recsize);
		sleep(1);
		//printf("datagram: %s\n",buf);

		// 576 байт гарантировано дойдет
	}

	// socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	// if (socket_fd == -1)
	// {
	// 	perror("Ошибка: создания сокета");
	// 	exit(EXIT_FAILURE);
	// }
	// memset(&st_sock_addr, 0, sizeof(st_sock_addr));
	// st_sock_addr.sin_family = AF_INET;
	// st_sock_addr.sin_port = htons(PORT);
	// st_sock_addr.sin_addr.s_addr = htons(inet_pton(AF_INET, my_ip, ))
	// if (bind(socket_fd, (struct sockaddr *) &st_sock_addr, sizeof(st_sock_addr)) == -1)
	// {
	// 	perror("Ошибка: связывания");
	// 	close(socket_fd);
	// 	exit(EXIT_FAILURE);
	// }
	// if (listen(socket_fd, 128) == -1) //listen(socket_fd, длина очереди)
	// {
	// 	perror("Ошибка: прослушивания");
	// 	close(socket_fd);
	// 	exit(EXIT_FAILURE);
	// }
	// while (1)
	// {

	// }
	// size_t recv(int fd, void *buf, size_t count, int flags);
	// size_t send(int fd, const void *buf, size_t count, int flags); // flag MSG_NOSIGNAL 

	// printf("my_ip %i:%i:%i:%i\n", g_c->my_ip[0], g_c->my_ip[1], g_c->my_ip[2], g_c->my_ip[3]);
	// printf("hi_ip %i:%i:%i:%i\n", g_c->hi_ip[0], g_c->hi_ip[1], g_c->hi_ip[2], g_c->hi_ip[3]);
	// printf("answer %s\ndonains ", g_c->black_domain_answer);
	// t_blc_list	*buf = g_c->black_list;
	// while (buf != NULL)
	// {
	// 	printf("%s\n", buf->blc_donain);
	// 	buf = buf->next_domain;
	// }
	return (1);
}