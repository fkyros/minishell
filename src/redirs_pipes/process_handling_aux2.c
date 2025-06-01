/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_handling_aux2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:14:33 by gade-oli          #+#    #+#             */
/*   Updated: 2025/06/01 11:14:35 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	prepare_child_cmd(t_command *cmd, t_mini *mini)
{
	int	redir_failed;
	int	status;
	int	is_path;

	redir_failed = 0;
	if (apply_redirections(cmd))
		redir_failed = 1;
	if (cmd->argv[0] == NULL)
	{
		if (redir_failed)
			exit(1);
		exit(0);
	}
	if (redir_failed)
		exit(1);
	if (is_builtin(cmd->argv[0]))
	{
		status = execute_builtin(cmd, mini);
		exit(status);
	}
	if (ft_strchr(cmd->argv[0], '/'))
		is_path = 1;
	else
		is_path = 0;
	return (is_path);
}

static void	exec_or_exit(char **argv, char **envp, int is_path)
{
	exec_command(argv, envp);
	if (is_path)
	{
		if (errno == EACCES || errno == EISDIR)
			exit(COMMAND_NOT_EXECUTABLE);
		exit(COMMAND_NOT_FOUND);
	}
	exit(COMMAND_NOT_FOUND);
}

void	child_branch(int i, t_parse_result *res, t_mini *mini)
{
	t_command	*cmd;
	int			is_path;

	cmd = &res->commands[i];
	is_path = prepare_child_cmd(cmd, mini);
	exec_or_exit(cmd->argv, mini->our_env, is_path);
}

int	find_pid_index(pid_t *pids, pid_t pid, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pids[i] == pid)
			return (i);
		i++;
	}
	return (-1);
}
