#include "get_next_line.h"
#include "libft/libft.h"

static int		write_rest(t_buf *s_fdlist, char **line, int fd)
{
	int j;
	int i;

	j = 0;
	i = 0;
	if (s_fdlist)
	{
		while (s_fdlist->fd != fd && s_fdlist->next)
			s_fdlist = s_fdlist->next;
		if (s_fdlist->fd != fd)
			return (-1);
		while (s_fdlist->buf[i] && s_fdlist->buf[i] != '\n')
			i++;
		if (!(*line = (char*)malloc(sizeof(char) * i)))
			exit (-1);
		i = 0;
		while (s_fdlist->buf[i] && s_fdlist->buf[i] != '\n')
			(*line)[j++] = s_fdlist->buf[i++];
		(*line)[j] = '\0';
		if (s_fdlist->buf[i] == '\n')
		{
			s_fdlist->buf = &s_fdlist->buf[i + 1];
			return (1);
		}
		return (0);
	}
	return (-1);
}

static t_buf	*add_list(t_buf **s_fdlist, int fd)
{
	t_buf	*tmp;

	tmp = (t_buf*)malloc(sizeof(t_buf));
	tmp->fd = fd;
	tmp->buf = ft_strnew(BUF_SIZE);
	if (s_fdlist)
		tmp->next = *s_fdlist;
	else
		tmp->next = NULL;
	*s_fdlist = tmp;
	return (*s_fdlist);
}

int		get_next_line(const int fd, char **line)
{
	static t_buf	*s_fdlist;
	char			buf[BUF_SIZE + 1];
	int				rbyte; // количество считанных байт
	int				ret_value;

	if (fd < 0)
		return (-1);
	if ((ret_value = write_rest(s_fdlist, line, fd) != -1))
		return (ret_value);
	s_fdlist = add_list(&s_fdlist, fd);
	while ((rbyte = read(fd, buf, BUF_SIZE)))
	{
		buf[rbyte] = '\0';
		s_fdlist->buf = ft_strjoin(s_fdlist->buf, buf); // записываем все в s_fdlist->buf вместе с возможным \n 
	}
	// нужно записать в *line строку до \n или \0 и вернуть 1 или 0 соответственно
	if ((ret_value = write_rest(s_fdlist, line, fd) != -1))
		return (ret_value);
	return (0);
}

int		main(int argc, char **argv)
{
	char	*line;
	int		fd;
	int		fd1;

	if (argc == 0)
		return 0;
	printf("---------------------------------\n");

	fd = open(argv[1], O_RDONLY);	
//	printf("FD = %d\n", fd);
	get_next_line(fd, &line);
	printf("LINE = %s\n", line);

	printf("---------------------------------\n");
 
	fd1 = open(argv[2], O_RDONLY);
//	printf("%d\n", fd1);
	get_next_line(fd1, &line);
	printf("LINE - 2 = %s\n", line);

	printf("---------------------------------\n");

	get_next_line(fd, &line);
	printf("LINE = %s\n", line);

	printf("---------------------------------\n");

	get_next_line(fd1, &line);
	printf("LINE - 2 = %s\n", line);

	printf("---------------------------------\n");

	get_next_line(fd, &line);
	printf("LINE = %s\n", line);

	printf("---------------------------------\n");
 	
	return (0);
}
