/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:00:26 by gade-oli          #+#    #+#             */
/*   Updated: 2025/04/06 19:29:03 by event            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//TODO: check negative numbers or invalid values for funny corrections
char	*shlvl(char *lvl, char **new_env)
{
	int	nlvl;
	char	**split;
	char	*res;
	char	*lvl_aux;

	split = ft_split(lvl, '=');
	if (!split || !split[2])
		return (free_split(split), ft_strdup("SHLVL=0"));
	nlvl = ft_atoi(split[2]);
	lvl_aux = ft_itoa(++nlvl);
	res = ft_strjoin("SHLVL=", lvl_aux);
	free_split(split);
	free(lvl_aux);
	return (res);
}

char	**init_env(char **env)
{
	int	i;
	char	**res;

	i = 0;
	while (env[i])
		i++;
	res = (char **) ft_calloc(i + 1, sizeof(char *));
	if (!res)
		return (NULL); //TODO: implement and execute safe exit
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(res[i], "SHLVL=", 6)
			res = shlvl(env[i], res);
		else
			res = ft_strdup(env[i]);
		if (!res[i])
			return (NULL); //TODO: implement and execute safe exit
		i++;
	}
	return (res);
}

// TODO: implement this as a local get_env for new env
// what if just $ ?
// NOTE: only expands `env` variables atm, not $? yet
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
