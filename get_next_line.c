/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikryvenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/04 21:00:23 by ikryvenk          #+#    #+#             */
/*   Updated: 2017/01/06 16:07:15 by ikryvenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		while (*(s_fdlist->buf) == '\n')
			s_fdlist->buf++;
		while (s_fdlist->buf[i] && s_fdlist->buf[i] != '\n')
			i++;
		*line = (char*)malloc(sizeof(char) * i);
		while (*(s_fdlist->buf) && *(s_fdlist->buf) != '\n')
			(*line)[j++] = *(s_fdlist->buf++);
		(*line)[j] = '\0';
		if (!**line)
			return (0);
		return (1);
	}
	return (-1);
}

static t_buf	*add_list(t_buf **s_fdlist, int fd)
{
	t_buf	*tmp;

	tmp = (t_buf*)malloc(sizeof(t_buf));
	tmp->fd = fd;
	tmp->buf = ft_strnew(BUF_SIZE);
	if (*s_fdlist)
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
	char			*tmp;
	int				rbyte;
	int				ret_value;

	if (fd < 0)
		return (-1);
	if ((ret_value = write_rest(s_fdlist, line, fd)) != -1)
		return (ret_value);
	s_fdlist = add_list(&s_fdlist, fd);
	while ((rbyte = read(fd, buf, BUF_SIZE)) > 0)
	{
		buf[rbyte] = '\0';
		tmp = s_fdlist->buf;
		s_fdlist->buf = ft_strjoin(s_fdlist->buf, buf);
		free(tmp);
	}
	if (rbyte == -1)
		return (-1);
	if ((ret_value = write_rest(s_fdlist, line, fd)) != -1)
		return (ret_value);
	return (0);
}

/*
int		main(int argc, char **argv)
{
	char	*line;
	int		fd;
//	int		fd1;
//	int		fd2;

	fd = 1;
	write(fd, "111\n222\n", 8);
//	get_next_line(fd, &line);
//	printf("LINE - 1 = %s\n", line);
//	get_next_line(fd, &line);
//	printf("LINE - 1 = %s\n", line);
//	fd = open(argv[1], O_RDONLY);
	while (get_next_line(fd, &line))
		printf("LINE - 2 = %s\n", line);
	if (argc == 0)
		return 0;
	if (argv)
		return 0;
	printf("---------------------------------\n");

	fd = open(argv[1], O_RDONLY);	

	printf("FD = %d\n", fd);
	get_next_line(fd, &line);
	printf("LINE - 1 = %s\n", line);

	printf("---------------------------------\n");

	fd1 = open(argv[2], O_RDONLY);
	printf("FD1 = %d\n", fd1);
	get_next_line(fd1, &line);
	printf("LINE - 2 = %s\n", line);

	printf("---------------------------------\n");

	fd2 = open(argv[3], O_RDONLY);
	printf("FD2 = %d\n", fd2);
	get_next_line(fd2, &line);
	printf("LINE - 3 = %s\n", line);

	printf("---------------------------------\n");

	get_next_line(fd, &line);
	printf("LINE  1 = %s\n", line);

	printf("---------------------------------\n");

	get_next_line(fd1, &line);
	printf("LINE - 2 = %s\n", line);

	printf("---------------------------------\n");

	get_next_line(fd, &line);
	printf("LINE - 1 = %s\n", line);

	printf("---------------------------------\n");
 
	get_next_line(fd, &line);
	printf("LINE - 1 = %s\n", line);

	printf("---------------------------------\n");
	return (0);
}
*/
