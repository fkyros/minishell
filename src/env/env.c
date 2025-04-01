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

#include "../../inc/minishell.h"

// NOTE: only expands `env` variables atm, not $? yet
// char	**expand(char **matrix)
// {
// 	int	i;
// 	char	*var_content;
// 	char	*var_dup;
	
// 	i = 0;
// 	while (matrix[i])
// 	{
// 		if (ft_strncmp("$", matrix[i], 1) == 0)
// 		{
// 			matrix[i]++;
// 			var_content = getenv(matrix[i]);
// 			var_dup = ft_strdup(var_content);
// 			// free(matrix[i]);
// 			matrix[i] = var_dup;
// 		}
// 		i++;
// 	} 
// 	return (matrix);
// }

// TODO: implement this as a local get_env for new env
// what if just $ ?
char	*expand(char *var)
{
	char	*res;

	if (var && ft_strncmp(var, "$", 1) == 0 && var++)
	{
		res = getenv(var);
		return (ft_strdup(res));
	}
	return (NULL);
}