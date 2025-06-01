/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 10:33:48 by gade-oli          #+#    #+#             */
/*   Updated: 2025/06/01 10:50:55 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	*collect_exit_codes(t_parse_result *res, pid_t *pids)
{
	int		*exit_codes;
	int		status;
	int		i;
	int		remaining;
	pid_t	pid;

	exit_codes = alloc_and_zero_exit_codes(res->cmd_count);
	remaining = res->cmd_count;
	while (remaining > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid < 0)
			break ;
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
		i = find_pid_index(pids, pid, res->cmd_count);
		if (i == -1)
			continue ;
		update_exit_code(exit_codes, i, status);
		remaining--;
	}
	return (exit_codes);
}

static void	reap_and_handle_errors(t_parse_result *res,
			t_mini *mini, pid_t *pids)
{
	int	*exit_codes;
	int	last_index;

	last_index = res->cmd_count - 1;
	exit_codes = collect_exit_codes(res, pids);
	print_all_child_errors(res, exit_codes);
	mini->last_status = exit_codes[last_index];
	free(exit_codes);
}

static void	child_fd_setup(t_parse_result *res, int prev_fd, int fd[2], int i)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (i < res->cmd_count - 1)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	if (res->commands[i].heredoc_fd != -1)
	{
		dup2(res->commands[i].heredoc_fd, STDIN_FILENO);
		close(res->commands[i].heredoc_fd);
	}
}

static void	close_and_update_fds(int *prev_fd, int i,
				t_parse_result *res, int fd[2])
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (i < res->cmd_count - 1)
		close(fd[1]);
	if (i < res->cmd_count - 1)
		*prev_fd = fd[0];
	else
		*prev_fd = -1;
}

void	spawn_commands(t_parse_result *res, t_mini *mini)
{
	pid_t	*pids;
	int		prev_fd;
	int		fd[2];
	int		i;

	prev_fd = -1;
	pids = alloc_pids(res->cmd_count);
	i = 0;
	while (i < res->cmd_count)
	{
		if (i < res->cmd_count - 1)
			safe_pipe(fd);
		pids[i] = safe_fork();
		if (pids[i] == 0)
		{
			child_fd_setup(res, prev_fd, fd, i);
			child_branch(i, res, mini);
		}
		close_and_update_fds(&prev_fd, i, res, fd);
		i++;
	}
	reap_and_handle_errors(res, mini, pids);
	dup2(mini->saved_stdin, STDIN_FILENO);
	dup2(mini->saved_stdout, STDOUT_FILENO);
	free(pids);
}
