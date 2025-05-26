/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:27:15 by gade-oli          #+#    #+#             */
/*   Updated: 2025/05/09 19:29:37 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

void	execute_builtin(t_command *cmd, t_mini *mini)
{
	int	saved_stdin;
	int	saved_stdout;
	int redir_failed;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	redir_failed = 0;
	if (saved_stdin < 0 || saved_stdout < 0)
	{
		perror(BOLD RED"minishell: dup"RST);
		if (saved_stdin  >= 0) 
			close(saved_stdin);
        if (saved_stdout >= 0) 
			close(saved_stdout);
		return ;
	}
	if (apply_redirections(cmd) != 0)
    {
        mini->last_status = 1;
        redir_failed = 1;
    }
	if (!redir_failed)
	{
		if (!ft_strcmp(cmd->argv[0], "cd"))
			builtin_cd(cmd->argv, mini);
		else if (!ft_strcmp(cmd->argv[0], "echo"))
			builtin_echo(cmd->argv, mini);
		else if (!ft_strcmp(cmd->argv[0], "pwd"))
			builtin_pwd(cmd->argv, mini);
		else if (!ft_strcmp(cmd->argv[0], "env"))
			builtin_env(mini);
		else if (!ft_strcmp(cmd->argv[0], "export"))
			builtin_export(cmd->argv, mini);
		else if (!ft_strcmp(cmd->argv[0], "unset"))
			builtin_unset(cmd->argv, mini);
		else if (!ft_strcmp(cmd->argv[0], "exit"))
			builtin_exit(cmd->argv, mini);
	}
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		perror(BOLD RED"minishell: dup2"RST);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		perror(BOLD RED"minishell: dup2"RST);
	close(saved_stdin);
	close(saved_stdout);
}

int	get_num_args(char **args)
{
	int	res;

	res = 0;
	while (args[res])
		res++;
	return (res);
}
