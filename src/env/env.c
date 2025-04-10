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

char	*shlvl(char *lvl)
{
	int	nlvl;
	char	**split;
	char	*res;
	char	*lvl_aux;

	split = ft_split(lvl, '=');
	if (!split || !split[2])
		return (free_split(split), ft_strdup("SHLVL=0"));
	nlvl = ft_atoi(split[2]);
	nlvl++;
	lvl_aux = ft_itoa(nlvl);
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
		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
			res[i] = shlvl(env[i]);
		else
			res[i] = ft_strdup(env[i]);
		if (!res[i])
			return (NULL); //TODO: implement and execute safe exit
		i++;
	}
	return (res);
}

// var name should NOT include its '=' at the end of the name, since the function already handles that
char	*ft_getenv(char *var, char **env)
{
	char	*res;
	int	i;
	char	*real_var;
	int	size;
	int	found;

	if (!env || !var || !var[0])
		return (NULL);
	real_var = ft_strjoin(var, "=");
	size = ft_strlen(real_var);
	i = 0;
	found = 0;
	res = NULL;
	while (!found && env[i])
	{
		if (ft_strncmp(real_var, env[i], size) == 0)
			found = 1;
		i++;
	}
	if (found)
		res = ft_substr(env[i - 1], size, ft_strlen(env[i - 1] + 1));
	free(real_var);
	return (res);
}

// TODO: implement this as a local get_env for new env
// what if just $ ?
// NOTE: only expands `env` variables atm, not $? yet
char	*expand(char *var, char **our_env)
{
	if (var && ft_strncmp(var, "$", 1) == 0 && var++)
		return (ft_getenv(var, our_env));
	return (NULL);
}
