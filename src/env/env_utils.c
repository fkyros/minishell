/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 00:03:14 by gade-oli          #+#    #+#             */
/*   Updated: 2025/05/26 00:03:16 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	int		res;

	real_name = ft_strjoin(name, "=");
	res = ft_strncmp(real_name, var_from_env, ft_strlen(real_name));
	free(real_name);
	if (res == 0)
		return (1);
	return (0);
}

static char	**create_empty_env(char **our_env)
{
	char	**new_env;
	int		i;

	i = 0;
	while (our_env[i])
		i++;
	new_env = (char **) ft_calloc(i + 2, sizeof(char *));
	return (new_env);
}

char	**add_var_to_env(char **our_env, char *name, char *value)
{
	int		i;
	int		j;
	char	**new_env;

	new_env = create_empty_env(our_env);
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
	new_env[++j] = NULL;
	return (new_env);
}

char	**delete_var_from_env(char *name, char **our_env)
{
	char	**new_env;
	int		i;
	int		j;

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
