/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 09:36:52 by jorexpos          #+#    #+#             */
/*   Updated: 2024/09/26 10:00:46 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stddef.h>

int	string_count(char const *s1, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	if (*s1 == '\0')
		return (0);
	while (*s1 != '\0')
	{
		if (*s1 == c)
			in_word = 0;
		else if (in_word == 0)
		{
			in_word = 1;
			count++;
		}
		s1++;
	}
	return (count);
}

int	char_count(char const *s2, char c, int i)
{
	int	length;

	length = 0;
	while (s2[i] != c && s2[i] != '\0')
	{
		length++;
		i++;
	}
	return (length);
}

char	**free_arr(char **dst, int j)
{
	while (j > 0)
	{
		j--;
		free(dst[j]);
	}
	free(dst);
	dst = NULL;
	return (NULL);
}

char	**substring_handler(char const *s, char **dst, char c, int l)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	while (s[i] != '\0' && j < l)
	{
		k = 0;
		while (s[i] == c)
			i++;
		dst[j] = (char *)malloc(sizeof(char) * char_count(s, c, i) + 1);
		if (dst[j] == NULL)
			return (free_arr(dst, j));
		while (s[i] != '\0' && s[i] != c)
			dst[j][k++] = s[i++];
		dst[j][k] = '\0';
		j++;
	}
	dst[j] = NULL;
	return (dst);
}

char	**ft_split(char const *s, char c)
{
	char	**dst;
	int		l;

	if (s == NULL)
		return (NULL);
	l = string_count(s, c);
	dst = (char **)malloc(sizeof(char *) * (l + 1));
	if (dst == NULL)
		return (NULL);
	return (substring_handler(s, dst, c, l));
}
