
#include "DNS.h"

int		is_ip(char *line)
{
	while (*line != '\0')
	{
		if (*line == '.' || (*line >= '0' && *line <= '9'))
			line++;
		else
			return (0);
	}
	return (1);
}

void	parse_ip(char *line, int flag)///////////////////split
{
	char		**ip;

	ip = split(line, '.');
	for (int i = 0; i < 4; ++i)
	{
		flag == 0 ? g_c->my_ip[i] = atoi(ip[i]) : g_c->hi_ip[i] = atoi(ip[i]);
		if (ip[i])
			free(&ip[i]);
	}
	if (ip)
		free(&ip);
}

void	parse_config_info(char *str)
{
	char 		*line = NULL;
	int			flag = 0;
	t_blc_list	*buf1;
	t_blc_list	*buf2;

	while (get_next_line(fd, &line) == 1) // read && parse config_file line by line
	{
		if ((flag == 0 || flag == 1) && is_ip(line))//find string vs ip
			parse_ip(line, flag++);
		else if (flag == 2 && line[0] != '\0' && line[0] != '#' && line[1] != '#' && line[0] != '\n')//find string vs server answer
		{
			g_c->black_domain_answer = strdup(line);
			flag++;
		}
		else if (flag == 3 && line[0] != '\0' && line[0] != '#' && line[1] != '#' && line[0] != '\n')
		{
			buf2 = (t_blc_list *)malloc(sizeof(t_blc_list));
			buf2->next_domain = NULL;
			buf2->blc_donain = strdup(line);
			if (g_c->black_list == NULL)// if it's first domain, save head of linkedlists
				g_c->black_list = buf2;
			else
			{
				buf1 = g_c->black_list;
				while (buf1->next_domain != NULL)
					buf1 = buf1->next_domain;
				buf1->next_domain = buf2;
			}
		}
		strdel(&line);
	}
}

int		main(int argc, char **argv)
{
	int			fd = -1;
	t_config	*g_c;

	if (argc != 2) // if !config_file, print usage && exit
		return (usage());
	if (!(g_c = (t_config *)malloc(sizeof(t_config)))) //malloc memory for global variable
		return (0); //exit program, if memory not allocated
	fd = open(argv[1], O_RDONLY); // take fd for read config file
	parse_config_info(fd); // read config info
	close(fd); // close fd
}