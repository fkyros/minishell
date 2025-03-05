/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:23:22 by jorexpos          #+#    #+#             */
/*   Updated: 2025/01/16 12:23:23 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_gnl_strjoin(char *bm, char *buf)
{
	size_t	i;
	char	*str;
	char	*orig_bm;

	if (!bm || !buf)
		return (NULL);
	str = malloc(sizeof(char) * ((ft_strlen(bm)
					+ ft_strlen(buf)) + 1));
	if (!str)
	{
		free(bm);
		return (NULL);
	}
	orig_bm = bm;
	i = 0;
	while (*orig_bm)
		str[i++] = *orig_bm++;
	while (*buf)
		str[i++] = *buf++;
	str[i] = '\0';
	free(bm);
	return (str);
}

static char	*ft_init_buffer_and_bookmark(char **bookmark)
{
	char	*buffer;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	if (!*bookmark)
	{
		*bookmark = malloc(sizeof(char));
		if (!*bookmark)
		{
			free(buffer);
			return (NULL);
		}
		**bookmark = '\0';
	}
	return (buffer);
}

static int	ft_read_and_update_bookmark(int fd, char *buffer, char **bookmark)
{
	int	read_bytes;

	read_bytes = read(fd, buffer, BUFFER_SIZE);
	if (read_bytes == -1)
	{
		free(*bookmark);
		return (-1);
	}
	if (read_bytes > 0)
	{
		buffer[read_bytes] = '\0';
		*bookmark = ft_gnl_strjoin(*bookmark, buffer);
		if (!*bookmark)
			return (-1);
	}
	return (read_bytes);
}

char	*ft_read_to_bookmark(int fd, char *bookmark)
{
	char	*buffer;
	int		read_bytes;

	buffer = ft_init_buffer_and_bookmark(&bookmark);
	if (!buffer)
		return (NULL);
	read_bytes = 1;
	while (!ft_strchr(bookmark, '\n') && read_bytes != 0)
	{
		read_bytes = ft_read_and_update_bookmark(fd, buffer, &bookmark);
		if (read_bytes == -1)
		{
			free(buffer);
			return (NULL);
		}
	}
	free(buffer);
	return (bookmark);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*bookmark;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	bookmark = ft_read_to_bookmark(fd, bookmark);
	if (!bookmark)
		return (NULL);
	line = ft_extract_line(bookmark);
	bookmark = ft_update_bookmark(bookmark);
	if (!line && bookmark)
	{
		free(bookmark);
		return (NULL);
	}
	return (line);
}
