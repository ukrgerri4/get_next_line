/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikryvenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/26 20:42:47 by ikryvenk          #+#    #+#             */
/*   Updated: 2016/12/27 19:13:49 by ikryvenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int get_next_line(const int fd, char **line)
{
	static char	buf[BUF_SIZE + 1];
	char		*tmp;
	int			i;

	write(1, "1\n", 2);
	line = (char**)malloc(sizeof(*line) * 2);
	*line = (char*)malloc(sizeof(**line) * BUF_SIZE + 1);
	tmp = *line;
	write(1, "2\n", 2);
	if (read(fd, buf, BUF_SIZE) != 0)
	{
		write(1, "3\n", 2);
		i = 0;
		while (i < BUF_SIZE && buf[i] != '\n')
			*(tmp++) = buf[i++];
		*tmp = '\0';
		printf("%s", *line);
	}
	else
		return (-1);
	return (0);
}

int main(int argc, char **argv)
{
	char **line;
	int		fd;

	fd = 0;
	line = 0;
	if (argc == 2)
		fd = open(argv[1], O_RDONLY);
	get_next_line(fd, line);
	return (0);
}
