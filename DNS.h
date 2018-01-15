#ifndef DNS_H
# define DNS_H

# include <stdio.h>
# include <fcntl.h>

# define PORT 53

typedef struct			s_line
{
	char				*l;
	int					n_fd;
	struct s_line		*next;
}						t_line;

typedef struct			s_blc_list
{
	char				*blc_donain = NULL;
	t_blc_list			*next_domain = NULL;
}						t_blc_list;

typedef struct			s_config
{
	int					my_ip[4] = {0, 0, 0, 0};
	int					hi_ip[4] = {0, 0, 0, 0};
	char				*black_domain_answer = NULL;
	t_blc_list			*black_list = NULL;
}						t_config;

extern t_config			*g_c;

#endif