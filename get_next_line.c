/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikryvenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/11 23:10:32 by ikryvenk          #+#    #+#             */
/*   Updated: 2017/01/12 01:22:39 by ikryvenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int			write_rest(t_buf *s_fdlist, char **line, int fd)
{
	int	i;

	i = 0;
	if (s_fdlist)
	{
		while (s_fdlist->fd != fd && s_fdlist->next)
			s_fdlist = s_fdlist->next;
		if (s_fdlist->fd == fd && s_fdlist->buf)
		{
			while (*(s_fdlist->buf) && *(s_fdlist->buf) != '\n')
				(*line)[i++] = *(s_fdlist->buf++);
			if (*(s_fdlist->buf) == '\n')
			{
				s_fdlist->buf++;
				return (1);
			}
		}
	}
	return (0);
}

static int			add_list(t_buf **s_fdlist, char *buf, int fd)
{
	t_buf	*tmp;

	tmp = *s_fdlist;
	if (tmp)
	{
		while (tmp->fd != fd && tmp->next)
			tmp = tmp->next;
		if (tmp->fd == fd)
		{
			tmp->buf = ft_strcpy(tmp->buf, buf);
			return (1);
		}
	}
	tmp = (t_buf*)malloc(sizeof(t_buf));
	tmp->fd = fd;
	tmp->buf = ft_strnew(BUFF_SIZE + 1);
	tmp->buf = ft_strcpy(tmp->buf, buf);
	if (*s_fdlist)
		tmp->next = *s_fdlist;
	else
		tmp->next = NULL;
	*s_fdlist = tmp;
	return (1);
}

static char			*add_memory(char *line, int len, int i)
{
	char	*str;

	str = ft_strnew(len);
	str = ft_strncpy(str, line, i);
	ft_strdel(&line);
	return (str);
}

static int			write_line(t_buf **s_fdlist, char **line, int fd, int len)
{
	char	buf[BUFF_SIZE + 1];
	int		rbyte;
	int		i;

	i = 0;
	while ((*line)[i])
		i++;
	while ((rbyte = read(fd, buf, BUFF_SIZE)) > 0)
	{
		buf[rbyte] = '\0';
		rbyte = 0;
		while (buf[rbyte] && buf[rbyte] != '\n')
		{
			if (i == len)
				*line = add_memory(*line, len *= 2, i);
			(*line)[i++] = buf[rbyte++];
		}
		(*line)[i] = '\0';
		if (buf[rbyte] == '\n')
			return (add_list(s_fdlist, &buf[++rbyte], fd));
	}
	return (rbyte);
}

int					get_next_line(const int fd, char **line)
{
	static t_buf	*s_fdlist;
	int				rbyte;
	int				len;

	if (s_fdlist)
	if (fd < 0)
		return (-1);
	len = 50;
	*line = ft_strnew(len);
	if (write_rest(s_fdlist, line, fd))
		return (1);
	rbyte = write_line(&s_fdlist, line, fd, len);
	if (!rbyte && !**line)
		return (0);
	if (rbyte < 0)
		return (-1);
	return (1);
}
