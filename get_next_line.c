#include "get_next_line.h"

static int          write_rest(char *str, char **line, size_t n)
{
    char    *tmp;
    int     i;

    tmp = str;
    i = 0;
    if (ft_memchr(tmp, '\n', n))
    {
        tmp = str;
        while (*tmp++ != '\n')
            i++;
        if (*line)
        {
            tmp = ft_strnew(ft_strlen(*line) + i);
            tmp = ft_strcpy(tmp, *line);
            tmp = ft_strncat(tmp, str, i);
            *line = tmp;
        }
        else
            *line = ft_strsub(str, 0, i);
        str = ft_memmove(str, &str[i + 1], n - i);
        str[n - i] = '\0';
        return (1);
    } 
    if (*line)
        *line = ft_strjoin(*line, str);
    else
        *line = ft_strdup(str);
    ft_bzero(str, n);
    return (0);
}

static int			check_rest(t_buf *s_fdlist, char **line, int fd)
{
    if (*line)
        *line = 0;
    if (s_fdlist)
    {
        while (s_fdlist->fd != fd && s_fdlist->next)
            s_fdlist = s_fdlist->next;
        if (s_fdlist->fd == fd && s_fdlist->buf)
            if (write_rest(s_fdlist->buf, line, ft_strlen(s_fdlist->buf)))
                return (1);
     }   
    return (0);
}

static void		check_list(t_buf **s_fdlist, char *buf, int fd)
{
	t_buf	*tmp;

    tmp = *s_fdlist;
    if (tmp)
    {
        while (tmp->fd != fd && tmp->next)
            tmp = tmp->next;
        if (tmp->fd == fd)
        {
            tmp->buf = ft_strdup(buf);
            return ;
        }       
    }
	tmp = (t_buf*)malloc(sizeof(t_buf));
	tmp->fd = fd;
	tmp->buf = ft_strdup(buf);
	if (*s_fdlist)
		tmp->next = *s_fdlist;
	else
		tmp->next = NULL;
	*s_fdlist = tmp;
}

int					get_next_line(const int fd, char **line)
{
	static t_buf	*s_fdlist;
	char			buf[BUF_SIZE + 1];
	int				rbyte;

	if (fd < 0)
		return (-1);
    if (check_rest(s_fdlist, line, fd))
        return (1);
    while ((rbyte = read(fd, buf, BUF_SIZE)) > 0)
    {
        buf[rbyte] = '\0';
        if (write_rest(buf, line, rbyte) == 1)
        {
            check_list(&s_fdlist, buf, fd);
            return (1);
        }
    }
    if (rbyte == -1)
        return (-1);
    if (rbyte == 0)
        return (0);
	return (1);
}
