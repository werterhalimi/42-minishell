/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:01:46 by ncotte            #+#    #+#             */
/*   Updated: 2022/10/17 10:02:37 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <stdio.h>

/// @brief Search the start of the buffer (ignore the first \\0)
/// @return the address
static char	*start_buffer(char *buffer)
{
	int	i;

	i = 0;
	while (!buffer[i] && i < BUFFER_SIZE)
		i++;
	if (i == BUFFER_SIZE)
		return (buffer);
	return (buffer + i);
}

/// @brief Read fd and put the content and return value into buf
/// @return 0 on success, -1 if error
static int	read_buffer(t_stat_var	*buf, int fd)
{
	int	index;

	gnl_bzero(buf->buffer, BUFFER_SIZE);
	index = read(fd, buf->buffer, BUFFER_SIZE);
	if (index < 0)
		return (-1);
	if (index)
		buf->index = READING_FILE;
	else
		buf->index = END_OF_FILE;
	return (0);
}

/// @brief Concatenate src and len bytes of buffer into a string
/// @return the new string, NULL if error
static char	*fill_next_line(char *buffer, char *src, int len)
{
	char	*dst;
	int		i;
	int		j;

	dst = malloc(sizeof (*dst) * (gnl_strlen(src) + len + 1));
	if (dst)
	{
		i = 0;
		if (src)
		{
			while (src[i])
			{
				dst[i] = src[i];
				i++;
			}
		}
		j = 0;
		while (j < len)
		{
			dst[i++] = buffer[j];
			buffer[j++] = 0;
		}
		dst[i] = 0;
	}
	return (dst);
}

/// @brief Concatenate src and multiples buffer
///	(until \\n is found) into a string
/// @return the new string, NULL if error
static char	*fill_str(t_stat_var *buf, char *src, int fd)
{
	char	*dst;
	int		len;

	len = gnl_strchr(start_buffer(buf->buffer), '\n');
	while (len < 0 && buf->index == READING_FILE)
	{
		dst = gnl_strjoin(src, start_buffer(buf->buffer));
		free(src);
		src = dst;
		if (read_buffer(buf, fd))
		{
			free(dst);
			return (NULL);
		}
		len = gnl_strchr(start_buffer(buf->buffer), '\n');
	}
	if (len < 0)
		dst = gnl_strjoin(src, start_buffer(buf->buffer));
	else
		dst = fill_next_line(start_buffer(buf->buffer), src, len + 1);
	free(src);
	return (dst);
}

char	*get_next_line(int fd)
{
	static t_stat_var	buf[4096];

	if (fd < 0 || 4095 < fd)
		return (NULL);
	if (buf[fd].index == NEW_FILE && read_buffer(buf + fd, fd))
		return (NULL);
	return (fill_str(buf + fd, NULL, fd));
}
