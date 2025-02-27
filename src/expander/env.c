/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:00:26 by gade-oli          #+#    #+#             */
/*   Updated: 2025/02/27 17:40:52 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/minishell.h"

// NOTE: only expands `env` variables atm
char	**expand(char **matrix)
{
	char	**res;
	int		i;
	char	*var;
	char	*var_dup;
	
	i = 0;
	while (matrix[i])
	{
		if (ft_strncmp("$", matrix[i], 1) == 0)
		{
			matrix[i]++;
			var = getenv(matrix[i]);
			if (!var)
				return (NULL); //TODO: print debug error that env var doesnt exist & clean exit
			// substitute in matrix the expanded one
			var_dup = ft_strdup(var);
			free(matrix[i]);
			matrix[i] = var_dup;
		}
		i++;
	} 
	return (res);
}
