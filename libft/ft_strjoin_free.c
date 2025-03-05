/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:23:05 by jorexpos          #+#    #+#             */
/*   Updated: 2025/01/16 12:23:11 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strjoin_free_helper(char *s1, char *s2, int size1, int size2)
{
	char	*str;

	str = malloc(sizeof(char) * (size1 + size2 + 1));
	if (!str)
	{
		free(s1);
		s1 = NULL;
		return (NULL);
	}
	ft_memcpy(str, s1, size1);
	ft_memcpy(str + size1, s2, size2 + 1);
	free(s1);
	return (str);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	int	size1;
	int	size2;

	if (!s1)
	{
		s1 = (char *)malloc(sizeof(char));
		if (!s1)
			return (NULL);
		s1[0] = '\0';
	}
	if (!s2)
		s2 = "";
	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	return (ft_strjoin_free_helper(s1, s2, size1, size2));
}
