/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikryvenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/26 20:42:47 by ikryvenk          #+#    #+#             */
/*   Updated: 2016/12/29 13:33:44 by ikryvenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include "libft/libft.h"
/*
t_buf	write_rest(t_buf *fd_buf, char *tmpi, int fd)
{
	if (fd_buf)
	{
		while (tmp->fd != fd && tmp->next)
			tmp = tmp->next;
		if (tmp->fd != fd)
			return (tmp);
		tmp = strcpy(fd_buf->buf, fd);
	}
}
*/
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

char	*str_malloc(char *line, int len, int j)
{
	char *str;

	str = (char*)malloc((sizeof(char) * len));
	str = ft_strncpy(str, line, j);
	free(tmp);
	return (str);
}


int		get_next_line(const int fd, char **line)
{
	static t_buf	*fd_buf;	
	char			buf[BUF_SIZE + 1];
	char			j;
	int				i;
	int				len;

	len = 30;
	if (!(*line = (char*)malloc(sizeof(char) * len)) || fd < 0)
		return (-1);
	j = 0;/*need to check if "fd" used before, and return count of characters which I can write*/
	while (read(fd, buf, BUF_SIZE))
	{
		i = 0;
		while (buf[i] != '\n' && buf[i] != '\0')
		{
			if (j == len - 1)
				*line = tmp_realloc(*line, len *= 2, j);
			(*line)[j++] = buf[i++];
		}
		(*line)[j] = '\0';
		if (buf[i] == '\n')
		{
//			save_buf(&fd_buf, buf, ++i, fd);
			return (1);
		}
	}
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
