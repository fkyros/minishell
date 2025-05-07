#include "../inc/minishell.h"

static void compact_argv(char **argv, int length)
{
	int	write_pos;
	int read_pos;

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

static int is_pipe(const char *token)
{
    return (ft_strcmp(token, "|") == 0);
}

static int is_redirection(const char *token)
{
    return (ft_strcmp(token, ">") == 0 ||
            ft_strcmp(token, ">>") == 0 ||
            ft_strcmp(token, "<") == 0 ||
            ft_strcmp(token, "<<") == 0);
}

static enum e_redirect_type get_redirection_type(const char *token)
{
    if (ft_strcmp(token, ">") == 0)
        return (out);
    if (ft_strcmp(token, ">>") == 0)
        return (append);
    if (ft_strcmp(token, "<") == 0)
        return (in);
    return (heredoc);
}

static char **copy_original_tokens(char **args, int token_count)
{
    char **original_tokens;
    int     i;

    original_tokens = malloc((token_count + 1) * sizeof(char *));
    if (!original_tokens)
        return NULL;
    i = 0;
    while(i < token_count)
    {
        original_tokens[i] = args[i];
        i++;
    }
    original_tokens[token_count] = NULL;
    return original_tokens;
}

static void init_command(t_command *cmd, char **argv_start, int is_first)
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

static int init_parse_result(const char *input, t_parse_result *result, t_mini *mini)
{
	int actual_cmd_count;
	int i;

    result->args = parse_command(input, &result->token_count, mini);
    if (!result->args)
        return 0;
    result->original_tokens = copy_original_tokens(result->args, result->token_count);
    if (!result->original_tokens) 
	{
        free(result->args);
        return (0);
    }
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

static char *extract_raw_heredoc_delim(const char *input) 
{
    const char *heredoc_pos;
    const char *delim_start;
    const char *delim_end;
    
    heredoc_pos = strstr(input, "<<");
    if (!heredoc_pos) 
        return NULL;
    delim_start = heredoc_pos + 2;
    while (*delim_start && is_whitespace(*delim_start)) 
        delim_start++;
    delim_end = delim_start;
    while (*delim_end && !is_whitespace(*delim_end) && 
           *delim_end != '|' && *delim_end != '<' && *delim_end != '>') {
        delim_end++;
    }
    return ft_substr(delim_start, 0, delim_end - delim_start);
}

static int handle_redirection(t_command *cmd, char **args, int *i, const char *input) 
{
    enum e_redirect_type type;
    char *heredoc_eof;

    type = get_redirection_type(args[*i]);
    if (!args[*i + 1]) 
    {
        ft_putstr_fd("minishell: syntax error near unexpected token\n", STDERR_FILENO);
        return (0);
    }
    if (type == heredoc) 
    {
        heredoc_eof = extract_raw_heredoc_delim(input);
        add_redirect(cmd, heredoc, NULL, heredoc_eof);
    } 
    else 
        add_redirect(cmd, type, ft_strdup(args[*i + 1]), NULL);
    args[*i] = NULL;
    args[*i + 1] = NULL;
    *i += 2;
    return (1);
}

static int fill_command(char **args, int *i, t_command *cmd, const char *input)
{
    int start;
	int	cmd_length;

	start = *i;
    while (args[*i] && !is_pipe(args[*i])) 
	{
        if (is_redirection(args[*i]))
        {
            if(!handle_redirection(cmd, args, i, input))
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

t_parse_result parse_commands(const char *input, t_mini *mini)
{
    t_parse_result result;
    int i;
	int prev_i;
    
	result = (t_parse_result){0};
	if (!init_parse_result(input, &result, mini))
		return (result);
	i = 0;
    while (result.args[i] && result.cmd_count < result.token_count)
	{
    	while (result.args[i] && ft_strcmp(result.args[i], "|") == 0) 
		{
        	ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
        	i++;
    	}
		if (!result.args[i]) 
			break ;		
		init_command(&result.commands[result.cmd_count], 
				&result.args[i], result.cmd_count == 0);
		prev_i = i;
		if (!fill_command(result.args, &i, &result.commands[result.cmd_count], input))
            return (result);
		if (!result.commands[result.cmd_count].argv[0] && 
			result.commands[result.cmd_count].redir_count == 0) 
		{
			ft_putstr_fd("minishell: syntax error: empty command\n", STDERR_FILENO);
			result.cmd_count = 0;
			break ;
    	}
    	if (i > prev_i)
        	result.cmd_count++;
	}
    return (result);
}