/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:00:26 by gade-oli          #+#    #+#             */
/*   Updated: 2025/05/09 19:24:20 by gade-oli         ###   ########.fr       */
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
		return (free_array(split), ft_strdup("SHLVL=0"));
	nlvl = ft_atoi(split[1]);
	nlvl++;
	lvl_aux = ft_itoa(nlvl);
	res = ft_strjoin("SHLVL=", lvl_aux);
	free_array(split);
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
		return (NULL);
	i = 0;
	while (old_env[i])
	{
		if (ft_strncmp(old_env[i], "SHLVL=", 6) == 0)
			res[i] = shlvl(old_env[i]);
		else if (ft_strncmp(old_env[i], "SHELL=", 6) == 0)
			res[i] = ft_strdup("SHELL=minishell");
		else
			res[i] = ft_strdup(old_env[i]);
		if (!res[i])
			return (NULL);
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

char	*expand(char *var, t_mini *mini)
{
	if (var && ft_strncmp(var, "$", 1) == 0 && !var[1])
		return (ft_strdup(var));
	if (var && ft_strncmp(var, "$?", 2) == 0)
		return (ft_itoa(mini->last_status));
	if (var && ft_strncmp(var, "$", 1) == 0 && var++)
		return (ft_getenv(var, mini->our_env));
	return (NULL);
}

char	*create_var_env(char *name, char *value)
{
	char	*aux;
	char	*res;

	if (!name)
		return (NULL);
	aux = ft_strjoin(name, "=");
	if (value)
	{
		res = ft_strjoin(aux, value);
		free(aux);
		return (res);
	}
	return (aux);
}

int	is_var_already_in_env(char *name, char *var_from_env)
{
	char	*real_name;
	int 	res;

	real_name = ft_strjoin(name, "=");
	res = ft_strncmp(real_name, var_from_env, ft_strlen(real_name));
	free(real_name);	
	if (res == 0)
		return (1);
	return (0);
}

char	**add_var_to_env(char **our_env, char *name, char *value)
{
	int	i;
	int	j;
	char	**new_env;

	i = 0;
	while (our_env[i])
		i++;
	new_env = (char **) ft_calloc(i + 2, sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (our_env[i])
	{
		if (!is_var_already_in_env(name, our_env[i]))
		{
			new_env[j] = ft_strdup(our_env[i]);
			if (!new_env[j])
				return (NULL);
			j++;
		}
		i++;
	}
	new_env[j] = create_var_env(name, value);
	if (!new_env[j])
		return (NULL);
	j++;
	new_env[j] = NULL;
	return (new_env);
}

char	**delete_var_from_env(char *name, char **our_env)
{
	char	**new_env;
	int	i;
	int	j;

	i = 0;
	while (our_env[i])
		i++;
	new_env = (char **) ft_calloc(i, sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (our_env[i])
	{
		if (!is_var_already_in_env(name, our_env[i]))
		{
			new_env[j] = ft_strdup(our_env[i]);
			if (!new_env[j])
				return (NULL);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}
