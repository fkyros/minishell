/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 09:40:25 by gade-oli          #+#    #+#             */
/*   Updated: 2025/05/25 20:22:31 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int is_valid_n_flag(char *arg)
{
	int i;

	i = 1;
	if (arg[0] != '-' || arg[1] != 'n')
		return (0);
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}

static int	echo_args(char **args, int *i)
{
	int	newline;

	newline = 1;
	while (args[*i] && is_valid_n_flag(args[*i]))
	{
		newline = 0;
		(*i)++;
	}
	return (newline);
}

void builtin_echo(char **args, t_mini *mini)
{
	int	i;
	int	newline;

	i = 1;
	newline = echo_args(args, &i);
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[(i++) + 1])
			write(1, " ", 1);
	}
	if (newline)
		write(1, "\n", 1);
	mini->last_status = 0;
}

void	update_cd_vars(t_mini *mini)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;
	char	**new_env;

	oldpwd = ft_getenv("PWD", mini->our_env);
	new_env = add_var_to_env(mini->our_env, "OLDPWD", oldpwd);
	free(oldpwd);
	free_array(mini->our_env);
	mini->our_env = new_env;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		perror(BOLD RED"minishell error: pwd"RST);
	new_env = add_var_to_env(mini->our_env, "PWD", cwd);
	free_array(mini->our_env);
	mini->our_env = new_env;
	mini->last_status = 0;
}


void	builtin_cd(char **args, t_mini *mini)
{
	char	*home;
	int	error;

	error = 0;
	if (!args[1])
	{
		home = ft_getenv("HOME", mini->our_env);
		if (!home)
			ft_putstr_fd(BOLD RED"Minishell: cd: HOME not set\n"RST, STDERR_FILENO);
		error = 1;
		if (safe_chdir(home) != 0)
			error = 1;
	}
	else
	{
		if (get_num_args(args) > 2)
		{
			ft_putstr_fd(BOLD RED"Minishell: cd: too many arguments\n"RST, STDERR_FILENO);
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

void builtin_pwd(t_mini *mini)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		perror(BOLD RED"minishell error: pwd"RST);
	printf("%s\n", cwd);
	mini->last_status = 0;
}
