/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:23:29 by jorexpos          #+#    #+#             */
/*   Updated: 2025/01/16 12:23:31 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_extract_line(char *bm)
{
	int		i;
	char	*str;

	i = 0;
	if (!bm || !bm[i])
		return (NULL);
	while (bm[i] && bm[i] != '\n')
		i++;
	str = (char *)malloc(sizeof(char) * (i + 2));
	if (!str)
		return (NULL);
	i = 0;
	while (bm[i] && bm[i] != '\n')
	{
		str[i] = bm[i];
		i++;
	}
	if (bm[i] == '\n')
		str[i++] = '\n';
	str[i] = '\0';
	return (str);
}

char	*ft_update_bookmark(char *bm)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	while (bm[i] && bm[i] != '\n')
		i++;
	if (!bm[i])
	{
		free(bm);
		return (NULL);
	}
	str = malloc(sizeof(char) * (ft_strlen(bm) - i + 1));
	if (!str)
	{
		free(bm);
		return (NULL);
	}
	i++;
	j = 0;
	while (bm[i])
		str[j++] = bm[i++];
	str[j] = '\0';
	free(bm);
	return (str);
}
