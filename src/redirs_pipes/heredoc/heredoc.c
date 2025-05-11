#include "../../../inc/minishell.h"

static int check_expand(const char *str)
{
    size_t len = ft_strlen(str);

    if (len >= 2
     && ((str[0] == '\'' && str[len-1] == '\'')
      || (str[0] == '"'  && str[len-1] == '"')))
        return (0);
    return (1);
}

static char *strip_quotes(const char *str)
{
    size_t len = ft_strlen(str);

    if (len >= 2
     && ((str[0] == '\'' && str[len-1] == '\'')
      || (str[0] == '"'  && str[len-1] == '"')))
    {
        return (ft_substr(str, 1, len - 2));
    }
    return (ft_strdup(str));
}

static int heredoc_readline(const char *eof, int expand, int fd, t_mini *mini)
{
	char *line;
    char *tmp;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, eof) == 0)
			break ;
		if (expand)
		{
			tmp = line;
			line = expand_line(tmp, mini);
			free(tmp);
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	return (0);
}

static void process_heredoc(t_command *cmd, const char *heredoc_eof, t_mini *mini)
{
    int pipe_fd[2];
    char *delim;
    int expand;

    if (pipe(pipe_fd) < 0)
    {
        perror("minishell: pipe");
        return ;
    }
    expand = check_expand(heredoc_eof);
    delim = strip_quotes(heredoc_eof);
    heredoc_readline(delim, expand, pipe_fd[1], mini);
    free(delim);
    close(pipe_fd[1]);
    cmd->heredoc_fd = pipe_fd[0];
}

void check_heredocs(t_parse_result *result, t_mini *mini)
{
    t_command   *cmd;
    int         i;
    int         j;

    if (!result || !result->commands)
        return ;
    i = 0;
    while (i < result->cmd_count)
    {
        cmd = &result->commands[i];
        cmd->heredoc_fd = -1;
        j = 0;
        while (j < cmd->redir_count)
        {
            if (cmd->redirs[j].type == heredoc)
            {
                process_heredoc(cmd,
                                cmd->redirs[j].heredoc_eof,
                                mini);
            }
            j++;
        }
        i++;
    }
}

void close_heredocs(t_parse_result *result)
{
	int i;

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
