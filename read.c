#include "DNS.h"

int		get_next_line(const int fd, char **line);
void	ft_strdel(char **ap);

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

void	parse_config_info(int fd)
{
	char 		*line = NULL;
	int			flag = 0;
	t_blc_list	*buf1;
	t_blc_list	*buf2;

	while (get_next_line(fd, &line) == 1) // read && parse config_file line by line
	{
		if ((flag == 0 || flag == 1) && is_ip(line) && line[0] != '\0' && line[0] != '\n')//find string vs ip
		{
			flag == 0 ? (g_c->my_ip = strdup(line)) : (g_c->hi_ip = strdup(line));
			flag++;
		}
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
		ft_strdel(&line);
	}
}
