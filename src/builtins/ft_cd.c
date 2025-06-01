/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 09:40:25 by gade-oli          #+#    #+#             */
/*   Updated: 2025/05/28 14:19:36 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	var_on_env(char *name, char **env)
{
	int	i;
	int	res;

	if (!name || !env)
		return (0);
	i = 0;
	res = 0;
	while (env[i] && !res)
	{
		if (is_var_already_in_env(name, env[i]))
			res = 1;
		i++;
	}
	return (res);
}

void	update_cd_vars(t_mini *mini)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;
	char	**new_env;

	if (var_on_env("OLDPWD", mini->our_env))
	{
		oldpwd = ft_getenv("PWD", mini->our_env);
		new_env = add_var_to_env(mini->our_env, "OLDPWD", oldpwd);
		free(oldpwd);
		free_array(mini->our_env);
		mini->our_env = new_env;
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		perror(BOLD RED"cd: error retrieving cwd"RST);
	if (var_on_env("PWD", mini->our_env))
	{
		new_env = add_var_to_env(mini->our_env, "PWD", cwd);
		free_array(mini->our_env);
		mini->our_env = new_env;
	}
	mini->last_status = 0;
}

static int	cd_without_args(t_mini *mini)
{
	int		error;
	char	*home;

	home = ft_getenv("HOME", mini->our_env);
	if (!home)
		ft_putstr_fd(BOLD RED"Minishell: cd: HOME not set\n"RST,
			STDERR_FILENO);
	error = 1;
	if (safe_chdir(home) != 0)
		error = 1;
	return (error);
}

int	get_num_args(char **args)
{
	int	res;

	res = 0;
	while (args[res])
		res++;
	return (res);
}

void	builtin_cd(char **args, t_mini *mini)
{
	int	error;

	error = 0;
	if (!args[1])
		error = cd_without_args(mini);
	else
	{
		if (get_num_args(args) > 2)
		{
			ft_putstr_fd(BOLD RED"Minishell: cd: too many arguments\n"RST,
				STDERR_FILENO);
			error = 1;
		}
		else if (safe_chdir(args[1]) != 0)
			error = 1;
	}
	if (error)
	{
		mini->last_status = GENERIC_ERROR;
		return ;
	}
	update_cd_vars(mini);
}
