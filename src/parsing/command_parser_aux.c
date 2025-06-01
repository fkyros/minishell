/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser_aux.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:43:31 by jorexpos          #+#    #+#             */
/*   Updated: 2025/06/01 16:43:33 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	compact_argv(char **argv, int length)
{
	int	write_pos;
	int	read_pos;

	if (!argv)
		return ;
	write_pos = 0;
	read_pos = 0;
	while (read_pos < length)
	{
		if (argv[read_pos] != NULL)
			argv[write_pos++] = argv[read_pos];
		read_pos++;
	}
	argv[write_pos] = NULL;
}

static char	**copy_original_tokens(char **args, int token_count)
{
	char	**original_tokens;
	int		i;

	original_tokens = malloc((token_count + 1) * sizeof(char *));
	if (!original_tokens)
		return (NULL);
	i = 0;
	while (i < token_count)
	{
		original_tokens[i] = args[i];
		i++;
	}
	original_tokens[token_count] = NULL;
	return (original_tokens);
}

void	init_command(t_command *cmd, char **argv_start, int is_first)
{
	cmd->argv = argv_start;
	cmd->redirs = NULL;
	cmd->redir_count = 0;
	cmd->heredoc = NULL;
	cmd->pipe_out = 0;
	cmd->is_first = is_first;
	cmd->is_last = 0;
	cmd->heredoc_fd = -1;
	ft_memset(&cmd->redirs, 0, sizeof(t_redirect *));
}

static int	count_commands(t_parse_result *result)
{
	int	actual_cmd_count;
	int	i;

	actual_cmd_count = 1;
	i = 0;
	while (result->args[i])
	{
		if (ft_strcmp(result->args[i], "|") == 0)
			actual_cmd_count++;
		i++;
	}
	result->commands = ft_calloc(actual_cmd_count + 1, sizeof(t_command));
	if (!result->commands)
	{
		free(result->args);
		return (0);
	}
	result->cmd_count = 0;
	return (1);
}

int	init_parse_result(const char *input,
	t_parse_result *result, t_mini *mini)
{
	result->args = parse_command(input, &result->token_count, mini);
	if (!result->args)
		return (0);
	result->original_tokens = copy_original_tokens(result->args,
			result->token_count);
	if (!result->original_tokens)
	{
		free(result->args);
		return (0);
	}
	return (count_commands(result));
}
