/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser_errors.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:43:36 by jorexpos          #+#    #+#             */
/*   Updated: 2025/06/01 16:43:37 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	process_fill_and_empty(t_parse_result *res,
	int *i, const char *input)
{
	t_command	*cmd;

	cmd = &res->commands[res->cmd_count - 1];
	if (!fill_command(res->args, i, cmd, input))
	{
		free_commands(res);
		return (-1);
	}
	if (!cmd->argv[0] && cmd->redir_count == 0)
	{
		ft_putstr_fd("minishell: syntax error: empty command\n", STDERR_FILENO);
		res->cmd_count = 0;
		return (0);
	}
	return (1);
}

int	check_extra_pipes(char **args, int i)
{
	if (args[i] && ft_strcmp(args[i], "|") == 0
		&& args[i + 1] && ft_strcmp(args[i + 1], "|") == 0)
	{
		return (1);
	}
	return (0);
}

int	check_invalid_pipes(t_parse_result *result, int *i)
{
	char	**args;

	args = result->args;
	if ((args[*i] && ft_strcmp(args[*i], "|") == 0)
		|| check_extra_pipes(args, *i))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
			STDERR_FILENO);
		result->cmd_count = 0;
		return (1);
	}
	return (0);
}
