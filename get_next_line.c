/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikryvenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/26 20:42:47 by ikryvenk          #+#    #+#             */
/*   Updated: 2016/12/28 17:03:40 by ikryvenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_buf	*add_struct(t_buf **fd_buf, int fd)
{
	t_buf	*tmp;

	if (fd_buf)
	{
		tmp = (t_buf*)malloc(sizeof(t_buf));
		tmp->fd = fd;
		tmp->buf = (char*)malloc(sizeof(char) * BUF_SIZE + 1);
		tmp->next = *fd_buf;
		*fd_buf = tmp;
	}
	else
	{
		tmp = (t_buf*)malloc(sizeof(t_buf));
		tmp->fd = fd;
		tmp->buf = (char*)malloc(sizeof(char) * BUF_SIZE + 1);
		tmp->next = NULL;
		*fd_buf = tmp;
	}
	return (*fd_buf);
}

void	save_buf(t_buf **fd_buf, char *buf, int i, int fd)
{
	t_buf	*tmp;
	int		j;

	j = 0;
	tmp = *fd_buf;
	if (tmp)
	{
		while (tmp->fd != fd && tmp->next)
			tmp = tmp->next;
		if (tmp->fd != fd)
			tmp = add_struct(fd_buf, fd);
		while (i < BUF_SIZE && buf[i])
			tmp->buf[j++] = buf[i++];
	}
	else
	{
		tmp = add_struct(fd_buf, fd);
		while (i < BUF_SIZE && buf[i])
			tmp->buf[j++] = buf[i++];
	}
	tmp->buf[j] = '\0';
}

int		get_next_line(const int fd, char **line)
{
	static t_buf	*fd_buf;
	static char		buf[BUF_SIZE + 1];
	char			*tmp;
	int				i;

	tmp = (char*)malloc(sizeof(*tmp) * BUF_SIZE + 1);
	*line = tmp;
	while (read(fd, buf, BUF_SIZE) != 0)
	{
		i = 0;
		while (buf[i] != '\n' && buf[i] != '\0')
			*(tmp++) = buf[i++];
		if (buf[i] == '\n')
		{
			save_buf(&fd_buf, buf, ++i, fd);
			return (1);
		}
	}
	*tmp = '\0';
	return (0);
}

int		main(int argc, char **argv)
{
	char	*line;
	int		fd;
	int		fd1;

	fd = open(argv[1], O_RDONLY);
	printf("FD = %d\n", fd);
	get_next_line(fd, &line);
	printf("LINE = %s\n", line);

/*	fd1 = open(argv[2], O_RDONLY);
	printf("%d\n", fd1);
	get_next_line(fd1, &line);
	printf("%s\n", line);*/

	get_next_line(fd, &line);
	printf("%s\n", line);
/*
	get_next_line(fd1, &line);
	printf("%s\n", line);
*/
	return (0);
}
