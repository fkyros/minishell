/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:27:15 by gade-oli          #+#    #+#             */
/*   Updated: 2025/05/28 01:17:02 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}

static void	builtin_switch(t_command *cmd, t_mini *mini)
{
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		builtin_cd(cmd->argv, mini);
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		builtin_echo(cmd->argv, mini);
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		builtin_pwd(mini);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		builtin_env(mini);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		builtin_export(cmd->argv, mini);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		builtin_unset(cmd->argv, mini);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		builtin_exit(cmd->argv, mini);
	else
		mini->last_status = 127;
}

static int	return_to_original_fds(int saved_stdin, int saved_stdout)
{
	safe_dup2(saved_stdin, STDIN_FILENO);
	safe_dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (1);
}

int	execute_builtin(t_command *cmd, t_mini *mini)
{
	int	saved_stdin;
	int	saved_stdout;
	int	redir_failed;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0)
	{
		perror("minishell: dup");
		if (saved_stdin >= 0)
			close(saved_stdin);
		if (saved_stdout >= 0)
			close(saved_stdout);
		mini->last_status = 1;
		return (1);
	}
	redir_failed = 0;
	if (apply_redirections(cmd) != 0)
	{
		mini->last_status = 1;
		redir_failed = 1;
	}
	if (redir_failed)
		return (return_to_original_fds(saved_stdin, saved_stdout));
	builtin_switch(cmd, mini);
	return_to_original_fds(saved_stdin, saved_stdout);
	return (mini->last_status);
}

int	get_num_args(char **args)
{
	int	res;

	res = 0;
	while (args[res])
		res++;
	return (res);
}
