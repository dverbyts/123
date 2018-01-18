#ifndef DNS_H
# define DNS_H

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
//сеть
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>

# define PORT 53
# define BUFF_SIZE 1

typedef struct			s_line
{
	char				*l;
	int					n_fd;
	struct s_line		*next;
}						t_line;

typedef struct			s_blc_list
{
	char				*blc_donain;
	struct s_blc_list	*next_domain;
}						t_blc_list;

typedef struct			s_config
{
	char				*my_ip;
	char				*hi_ip;
	char				*black_domain_answer;
	struct s_blc_list	*black_list;
}						t_config;

extern t_config			*g_c;




#endif