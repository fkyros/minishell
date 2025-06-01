/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:44:18 by jorexpos          #+#    #+#             */
/*   Updated: 2025/06/01 16:44:19 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	heredoc_readline(const char *eof, int expand, int fd, t_mini *mini)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_heredoc_warning(eof);
			break ;
		}
		if (ft_strcmp(line, eof) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(line, expand, fd, mini);
	}
	return (0);
}

static int	process_heredoc(t_command *cmd,
	const char *heredoc_eof, t_mini *mini)
{
	int		pipe_fd[2];
	char	*delim;
	int		expand;
	pid_t	pid;

	if (pipe(pipe_fd) < 0)
	{
		perror("minishell: pipe");
		return (-1);
	}
	expand = check_expand(heredoc_eof);
	delim = strip_quotes(heredoc_eof);
	pid = fork();
	if (pid < 0)
		return (heredoc_fork_error(delim, pipe_fd[0], pipe_fd[1]));
	else if (pid == 0)
	{
		close(pipe_fd[0]);
		heredoc_child_process(delim, expand, pipe_fd[1], mini);
	}
	free(delim);
	close(pipe_fd[1]);
	return (heredoc_parent_after(pid, pipe_fd[0], cmd));
}

static int	process_command_heredocs(t_command *cmd, t_mini *mini)
{
	int	j;
	int	err;

	j = 0;
	while (j < cmd->redir_count)
	{
		if (cmd->redirs[j].type == heredoc)
		{
			err = process_heredoc(cmd, cmd->redirs[j].heredoc_eof, mini);
			if (err == 1)
			{
				mini->last_status = 130;
				return (1);
			}
		}
		j++;
	}
	return (0);
}

int	check_heredocs(t_parse_result *result, t_mini *mini)
{
	t_command	*cmd;
	int			i;

	if (!result || !result->commands)
		return (1);
	i = 0;
	while (i < result->cmd_count)
	{
		cmd = &result->commands[i];
		cmd->heredoc_fd = -1;
		if (process_command_heredocs(cmd, mini) == 1)
			return (1);
		i++;
	}
	return (0);
}

void	close_heredocs(t_parse_result *result)
{
	int	i;

	i = 0;
	while (i < result->cmd_count)
	{
		if (result->commands[i].heredoc_fd != -1)
		{
			close(result->commands[i].heredoc_fd);
			result->commands[i].heredoc_fd = -1;
		}
		i++;
	}
}
