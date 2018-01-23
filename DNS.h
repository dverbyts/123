#ifndef DNS_H
# define DNS_H

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
//сеть


# define PORT 12345
# define ANY_PORT 12345
# define BUFF_SIZE 1
# define TIME_OUT 32767

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