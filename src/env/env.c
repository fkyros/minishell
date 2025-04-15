/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:00:26 by gade-oli          #+#    #+#             */
/*   Updated: 2025/04/15 19:28:15 by gade-oli         ###   ########.fr       */
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
	if (!split || !split[1])
		return (free_split(split), ft_strdup("SHLVL=0"));
	nlvl = ft_atoi(split[1]);
	nlvl++;
	lvl_aux = ft_itoa(nlvl);
	res = ft_strjoin("SHLVL=", lvl_aux);
	free_split(split);
	free(lvl_aux);
	return (res);
}

char	**init_env(char **old_env)
{
	int	i;
	char	**res;

	i = 0;
	while (old_env[i])
		i++;
	res = (char **) ft_calloc(i + 1, sizeof(char *));
	if (!res)
		return (NULL); //TODO: implement and execute safe exit
	i = 0;
	while (old_env[i])
	{
		if (ft_strncmp(old_env[i], "SHLVL=", 6) == 0)
			res[i] = shlvl(old_env[i]);
		else
			res[i] = ft_strdup(old_env[i]);
		if (!res[i])
			return (NULL); //TODO: implement and execute safe exit
		i++;
	}
	res[i] = NULL;
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

char	*create_var_env(char *name, char *value)
{
	char	*aux;
	char	*res;

	if (!name || !value)
		return (NULL);
	aux = ft_strjoin(name, "=");
	res = ft_strjoin(aux, value);
	free(aux);
	return (res);
}

char	**add_var_to_env(char **our_env, char *name, char *value)
{
	int	i;
	char	**new_env;

	i = 0;
	while (our_env[i]) //get_size_env
		i++;
	new_env = (char **) ft_calloc(i + 2, sizeof(char *));
	if (!new_env)
		return (NULL); //TODO: implement and execute safe exit
	i = 0;
	while (our_env[i])
	{
		new_env[i] = ft_strdup(our_env[i]);
		if (!new_env[i])
			return (NULL); //TODO: implement and execute safe exit
		i++;
	}
	new_env[i] = create_var_env(name, value);
	if (!new_env[i])
		return (NULL); //TODO: implement and execute safe exit
	i++;
	new_env[i] = NULL;
	return (new_env);
}
