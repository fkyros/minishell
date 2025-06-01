/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:22:11 by gade-oli          #+#    #+#             */
/*   Updated: 2025/06/01 11:32:45 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	add_redirect(t_command *cmd, enum e_redirect_type type,
		char *filename, char *heredoc_eof)
{
	t_redirect	*new_redirs;
	int			new_count;

	new_count = cmd->redir_count + 1;
	new_redirs = ft_realloc(cmd->redirs,
			cmd->redir_count * sizeof(t_redirect),
			new_count * sizeof(t_redirect));
	if (!new_redirs)
		return ;
	new_redirs[cmd->redir_count] = (t_redirect){
		.type = type,
		.filename = filename,
		.heredoc_eof = heredoc_eof
	};
	cmd->redirs = new_redirs;
	cmd->redir_count = new_count;
}

int	apply_redirections(t_command *cmd)
{
	int			status;
	int			fd;
	int			i;
	int			flags;
	t_redirect	*r;

	status = 0;
	i = 0;
	while (i < cmd->redir_count)
	{
		r = &cmd->redirs[i];
		if (r->type == in && r->filename)
		{
			fd = open(r->filename, O_RDONLY);
			if (fd < 0)
			{
				perror(r->filename);
				return (1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if ((r->type == out || r->type == append) && r->filename)
		{
			flags = O_WRONLY | O_CREAT;
			if (r->type == append)
				flags |= O_APPEND;
			else
				flags |= O_TRUNC;
			fd = open(r->filename, flags, 0644);
			if (fd < 0)
			{
				perror(r->filename);
				return (1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		i++;
	}
	return (status);
}
