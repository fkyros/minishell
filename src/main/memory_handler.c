/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:42:41 by jorexpos          #+#    #+#             */
/*   Updated: 2025/06/01 16:42:43 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	temp_size;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (malloc(new_size));
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (old_size > 0)
	{
		if (old_size < new_size)
			temp_size = old_size;
		else
			temp_size = new_size;
		ft_memcpy(new_ptr, ptr, temp_size);
		if (new_size > old_size)
			ft_bzero((char *)new_ptr + old_size, new_size - old_size);
	}
	free(ptr);
	return (new_ptr);
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_redirs(t_redirect *redirs, int redir_count)
{
	int	j;

	if (!redirs)
		return ;
	j = 0;
	while (j < redir_count)
	{
		if (redirs[j].type == heredoc && redirs[j].heredoc_eof)
			free(redirs[j].heredoc_eof);
		if (redirs[j].filename)
			free(redirs[j].filename);
		j++;
	}
	free(redirs);
}

static void	free_result_struct(t_parse_result *result)
{
	if (result->commands)
		free(result->commands);
	result->commands = NULL;
	result->args = NULL;
	result->original_tokens = NULL;
	result->cmd_count = 0;
	result->token_count = 0;
}

void	free_commands(t_parse_result *result)
{
	int			i;
	t_command	*cmd;

	if (!result)
		return ;
	if (result->original_tokens)
		free_array(result->original_tokens);
	if (result->args)
		free(result->args);
	i = 0;
	while (i < result->cmd_count)
	{
		cmd = &result->commands[i];
		if (cmd->redirs)
			free_redirs(cmd->redirs, cmd->redir_count);
		if (cmd->heredoc)
			free(cmd->heredoc);
		i++;
	}
	free_result_struct(result);
}
