/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:22:55 by gade-oli          #+#    #+#             */
/*   Updated: 2025/05/01 19:55:17 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	builtin_unset(char **args, t_mini *mini)
{
	int	i;
	int	j;
	int	flag;
	char	**new_env;

	i = 1;
	j = 0;
	flag = 0;
	new_env = NULL;
	while (args[i])
	{
		if (!ft_isalpha(args[i][0]))
		{
			i++;
			continue;
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
			free(mini->our_env);
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
	int	value_is_empty;

	value = ft_strchr(args[*i], '=');
	value++;
	value_is_empty = !value || !ft_strcmp(value, "");
	if (value_is_empty && args[*i+1] && !ft_strchr(args[*i+1], '='))
	{
		value_str_join = ft_strjoin(args[*i], args[*i+1]);
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
	int	i;
	char	**var;
	char	**new_env;
	int	status;

	status = 0;
	i = 1;
	if (!args[i])
	{
		ft_putstr_fd("no behaviour defined\n", STDERR_FILENO);
		ft_putstr_fd("see more at https://www.man7.org/linux/man-pages/man1/export.1p.html\n", STDERR_FILENO);
		return ;
	}
	while(args[i])
	{
		if (!ft_isalpha(args[i][0]) || !ft_strchr(args[i], '='))
		{
			ft_putstr_fd("error: export with an invalid identifier\n", STDERR_FILENO);
			status = 1;
		}
		else
		{
			var = export_args_split(args, &i);;
			new_env = add_var_to_env(mini->our_env, var[0], var[1]);
			if (!new_env)
				ft_putstr_fd("Error adding var to env\n", STDERR_FILENO);
			free_split(var);
			free_split(mini->our_env);
			mini->our_env = new_env;
		}
		i++;
	}
	mini->last_status = status;
}

void	builtin_env(t_mini *mini)
{
	int i;

	i = 0;
	while (mini->our_env[i])
	{
		printf("%s\n", mini->our_env[i]);
		i++;
	}
	mini->last_status = 0;
}

static int	ft_isnum(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-')
		i++;
	while (i)
	{
		if (!ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}

void	builtin_exit(char **args, t_mini *mini)
{
	int	status;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		mini->last_status = GENERIC_ERROR;
		return ;
	}
	if (!ft_isnum(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		status = MAX_ERROR;
	}
	else
		status = ft_atoi(args[1]) % 256;
	rl_clear_history();
	free_array(mini->our_env);
	free(mini);
	exit(status);
}
