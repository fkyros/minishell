#include "../inc/minishell.h"

static int	handle_redirection(t_command *cmd, char **args,
	int *i, const char *input)
{
	enum e_redirect_type	type;
	char					*raw_delim;

	type = get_redirection_type(args[*i]);
	if (!args[*i + 1])
	{
		ft_putstr_fd("minishell: syntax error near unexpected token \
			`newline'\n", STDERR_FILENO);
		return (0);
	}
	if (type == heredoc)
	{
		raw_delim = extract_raw_heredoc_delim(input, args[*i + 1]);
		add_redirect(cmd, heredoc, NULL, raw_delim);
	}
	else
		add_redirect(cmd, type, ft_strdup(args[*i + 1]), NULL);
	args[*i] = NULL;
	args[*i + 1] = NULL;
	*i += 2;
	return (1);
}

int	fill_command(char **args, int *i, t_command *cmd, const char *input)
{
	int	start;
	int	cmd_length;

	start = *i;
	while (args[*i] && !is_pipe(args[*i]))
	{
		if (is_redirection(args[*i]))
		{
			if (!handle_redirection(cmd, args, i, input))
				return (0);
		}
		else
			(*i)++;
	}
	if (args[*i] && ft_strcmp(args[*i], "|") == 0)
	{
		cmd->pipe_out = 1;
		args[(*i)++] = NULL;
	}
	cmd_length = *i - start;
	cmd->argv = &args[start];
	compact_argv(cmd->argv, cmd_length);
	return (1);
}

t_parse_result	parse_commands(const char *input, t_mini *mini)
{
	t_parse_result	result;
	int				i;
	int				status;

	result = (t_parse_result){0};
	if (!init_parse_result(input, &result, mini))
		return (result);
	i = 0;
	while (result.args[i] && result.cmd_count < result.token_count)
	{
		if (check_invalid_pipes(&result, &i) == 1)
			return (result);
		if (!result.args[i])
			break ;
		init_command(&result.commands[result.cmd_count],
			&result.args[i], (result.cmd_count == 0));
		result.cmd_count++;
		status = process_fill_and_empty(&result, &i, input);
		if (status < 0)
			return (result);
		else if (status == 0)
			break ;
	}
	return (result);
}
