/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 10:30:14 by gade-oli          #+#    #+#             */
/*   Updated: 2025/06/01 10:33:12 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	open_close_pipe(t_parse_result *result, int *i, int (*pipe_fd)[2])
{
	if (!result->commands[*i].is_last)
	{
		if (pipe(*pipe_fd) < 0)
		{
			perror(RED BOLD"minishell: pipe"RST);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		(*pipe_fd)[0] = -1;
		(*pipe_fd)[1] = -1;
	}
}

pid_t	*alloc_pids(int count)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * count);
	if (!pids)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (pids);
}

void	create_pipe(int i, int cmd_count, int pipes[2][2])
{
	if (i < cmd_count - 1 && pipe(pipes[i % 2]) < 0)
	{
		perror("minishell: pipe");
		exit(EXIT_FAILURE);
	}
}

void	execute_pipeline(t_parse_result *result, t_mini *mini)
{
	int	interrupted;

	interrupted = check_heredocs(result, mini);
	if (interrupted)
		return ;
	if (result->cmd_count == 1 && is_builtin(result->commands[0].argv[0]))
	{
		execute_builtin(&result->commands[0], mini);
		close_heredocs(result);
		return ;
	}
	spawn_commands(result, mini);
	close_heredocs(result);
}
