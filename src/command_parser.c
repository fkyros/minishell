#include "minishell.h"

// COPIES ORIGINAL TOKENS SO WE CAN FREE THEM AFTERWARDS
static char **copy_original_tokens(char **args, int token_count)
{
	int		i;
	char	**original_tokens;

	original_tokens = malloc(token_count * sizeof(char *));
	if (!original_tokens)
		return (NULL);
	i = 0;
	while (i < token_count)
	{
		original_tokens[i] = args[i];
		i++;
	}
	return (original_tokens);
}

// INITIALIZES t_parse_result STRUCT, COPYING AND ASSIGNING NECESSARY DATA AND MEMORY
static int	init_parse_result(const char *input, t_parse_result *result)
{
	result->args = parse_command(input, &result->token_count);
	if (!result->args)
		return (0);
	result->original_tokens = copy_original_tokens(result->args, result->token_count);
	if (!result->original_tokens)
	{
		free(result->args);
		return (0);
	}
	result->commands = malloc((result->token_count + 1) * sizeof(t_command));
	if (!result->commands)
	{
		free(result->args);
		free(result->original_tokens);
		return (0);
	}
	result->cmd_count = 0;
	return (1);
}

// INITIALIZES A COMMAND ASSIGNING DEFAULT VALUES
static void	init_command(t_command *cmd, char **argv_start, int is_first)
{
	cmd->argv = argv_start;
	cmd->redirect_in = NULL;
	cmd->heredoc = NULL;
	cmd->redirect_out = NULL;
	cmd->redirect_append = NULL;
	cmd->pipe_out = 0;
	cmd->is_first = is_first;
	cmd->is_last = 0;
}

// PROCESSES COMMAND TOKENS AND SEPARATES ARGUMENTS FROM OPERATORS (<, >, <<, >>)
static void	fill_command(char **args, int *i, t_command *cmd)
{
	// KEEPS GOING UNTIL IT FINDS AN OPERATOR OR IT'S THE END OF THE LINE
	while (args[*i] && strcmp(args[*i], "|") != 0 &&
		strcmp(args[*i], ">") != 0 && strcmp(args[*i], ">>") != 0 &&
		strcmp(args[*i], "<") != 0 && strcmp(args[*i], "<<") != 0)
		(*i)++;
	// PROCESSES OPERATORS AND ARGUMENTS
	while (args[*i])
	{
		if (!strcmp(args[*i], "|"))
		{
			cmd->pipe_out = 1;
			args[(*i)++] = NULL;
			break ;
		}
		else if (!strcmp(args[*i], ">>") && args[*i + 1])
		{
			cmd->redirect_append = args[*i + 1];
			args[(*i)++] = NULL;
			(*i)++;
		}
		else if (!strcmp(args[*i], ">") && args[*i + 1])
		{
			cmd->redirect_out = args[*i + 1];
			args[(*i)++] = NULL;
			(*i)++;
		}
		else if (!strcmp(args[*i], "<") && args[*i + 1])
		{
			cmd->redirect_in = args[*i + 1];
			args[(*i)++] = NULL;
			(*i)++;
		}
		else if (!strcmp(args[*i], "<<") && args[*i + 1])
		{
			cmd->heredoc = args[*i + 1];
			args[(*i)++] = NULL;
			(*i)++;
		}
		else
			break ;
	}
}

// MAIN FUNCTION THAT PROCESSES THE LINE AND BUILDS THE COMMANDS ARRAY
t_parse_result	parse_commands(const char *input)
{
	t_parse_result	result;
	int				i;
	int				cmd_start;

	result = (t_parse_result){0};
	if (!init_parse_result(input, &result))
		return (result);
	i = 0;
	cmd_start = 0;
	while (result.args[i] && result.cmd_count < result.token_count)
	{
        // SKIPS TOKENS THAT ARE ISOLATED OPERATORS
		while (result.args[i] && (ft_strcmp(result.args[i], "|") == 0 ||
			ft_strcmp(result.args[i], ">") == 0 ||
			ft_strcmp(result.args[i], ">>") == 0 ||
			ft_strcmp(result.args[i], "<") == 0 ||
			ft_strcmp(result.args[i], "<<") == 0))
			i++;
		if (!result.args[i])
			break ;
		init_command(&result.commands[result.cmd_count],
			&result.args[cmd_start], result.cmd_count == 0);
		fill_command(result.args, &i, &result.commands[result.cmd_count]);
		result.commands[result.cmd_count].is_last = (result.args[i] == NULL);
		result.cmd_count++;
		cmd_start = i;
	}
	return (result);
}