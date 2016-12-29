/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikryvenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/26 20:42:47 by ikryvenk          #+#    #+#             */
/*   Updated: 2016/12/29 19:18:04 by ikryvenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include "libft/libft.h"

int		write_rest(t_buf *fd_buf, char *tmp, int fd)
{
	int j;
	int i;

	j = 0;
	i = 0;
	if (fd_buf)
	{
		while (fd_buf->fd != fd && fd_buf->next)
			fd_buf = fd_buf->next;
		if (fd_buf->fd != fd)
			return (j);
		while (fd_buf->buf[i] && fd_buf->buf[i] != '\n')
			tmp[j++] = fd_buf->buf[i++];
		if (fd_buf->buf[i] == '\n')
		{
			tmp[j] = '\0';
			return (j = -1);
		}
		tmp[j] = '\0';
	}
	return (j);
}

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
		while (i > 0  && *buf)
		{
			tmp->buf[j++] = *buf++;
			i--;
		}
	}
	else
	{
		tmp = add_struct(fd_buf, fd);
		while (i > 0 && *buf)
		{
			tmp->buf[j++] = *buf++;
			i--;
		}
	}
	tmp->buf[j] = '\0';
}

char	*tmp_realloc(char *line, int len, int j)
{
	char *str;

	str = (char*)malloc((sizeof(char) * len + 1));
	str = ft_strncpy(str, line, j);
	free(line);
	return (str);
}


int		get_next_line(const int fd, char **line)
{
	static t_buf	*fd_buf;	
	char			buf[BUF_SIZE];
	char			j;
	int				i;
	int				len;
	int				rd;

	len = BUF_SIZE;
	if (!(*line = (char*)malloc(sizeof(char) * len + 1)) || fd < 0)
		return (-1);
	j = write_rest(fd_buf, *line, fd);
	printf("LINE_START = %s, `j = %d\n", *line, j);
	if (j != -1)
	{
		while ((rd = read(fd, buf, BUF_SIZE)))
		{
			i = 0;
			while (buf[i] != '\n' && buf[i] != '\0' && i < BUF_SIZE)
			{
				if (j == len - 1)
				{
					*line = tmp_realloc(*line, len *= 2, j);
				}
				(*line)[j++] = buf[i++];
			}
			(*line)[j] = '\0';
			if (buf[i] == '\n')
			{
				printf("rd = %d, i = %d\n", rd, i);
				save_buf(&fd_buf, &buf[i + 1], (rd - i - 1), fd);
				printf("fd_buf after rite = %s, fd_buf->fd = %d\n", fd_buf->buf, fd_buf->fd);
				return (1);
			}
		}
	}
	else
		return (1);
	return (0);
}

int		main(int argc, char **argv)
{
	char	*line;
	int		fd;
	int		fd1;

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
