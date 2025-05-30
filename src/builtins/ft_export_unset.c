/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:22:55 by gade-oli          #+#    #+#             */
/*   Updated: 2025/05/25 23:53:51 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	invalid_var_name(char *var)
{
	int	i;

	i = 0;
	if (!var)
		return (1);
	if (!ft_isalpha(var[0]))
		return (1);
	while (var[i])
	{
		if (ft_isalnum(var[i]) == 0 && var[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

static int	invalid_var_export(char *var)
{
	char	**name;
	int		res;

	name = ft_split(var, '=');
	res = invalid_var_name(name[0]);
	free_array(name);
	return (res);
}

void	builtin_unset(char **args, t_mini *mini)
{
	int		i;
	int		j;
	int		flag;
	char	**new_env;

	i = 1;
	j = 0;
	flag = 0;
	new_env = NULL;
	while (args[i])
	{
		if (invalid_var_name(args[i]) || !ft_strcmp(args[i], "_"))
		{
			i++;
			continue ;
		}
		while (!flag && mini->our_env[j])
		{
			if (is_var_already_in_env(args[i], mini->our_env[j]))
				flag = 1;
			j++;
		}
		if (flag)
			new_env = delete_var_from_env(args[i], mini->our_env);
		flag = 0;
		if (new_env)
		{
			free_array(mini->our_env);
			mini->our_env = new_env;
		}
		i++;
	}
	mini->last_status = 0;
}

char	**export_args_split(char **args, int *i)
{
	char	*value;
	char	*value_str_join;
	char	**res;
	int		value_is_empty;

	value = ft_strchr(args[*i], '=');
	value++;
	value_is_empty = !value || !ft_strcmp(value, "");
	if (value_is_empty && args[*i + 1] && !ft_strchr(args[*i + 1], '='))
	{
		value_str_join = ft_strjoin(args[*i], args[*i + 1]);
		res = ft_split(value_str_join, '=');
		free(value_str_join);
		(*i)++;
	}
	else
		res = ft_split(args[*i], '=');
	return (res);
}

void	builtin_export(char **args, t_mini *mini)
{
	int		i;
	char	**var;
	char	**new_env;
	int		status;

	status = 0;
	i = 1;
	if (!args[i])
	{
		ft_putstr_fd("no behaviour defined\n", STDERR_FILENO);
		return ;
	}
	while (args[i])
	{
		if (!ft_strncmp(args[i], "_=", 2))
		{
			i++;
			continue ;
		}
		else if (args[i][0] == '=' || !ft_strchr(args[i], '=')
			|| invalid_var_export(args[i]))
		{
			ft_putstr_fd("error: export with an invalid identifier\n",
				STDERR_FILENO);
			status = 1;
		}
		else
		{
			var = export_args_split(args, &i);
			new_env = add_var_to_env(mini->our_env, var[0], var[1]);
			if (!new_env)
				ft_putstr_fd("Error adding var to env\n", STDERR_FILENO);
			free_array(var);
			free_array(mini->our_env);
			mini->our_env = new_env;
		}
		i++;
	}
	mini->last_status = status;
}
