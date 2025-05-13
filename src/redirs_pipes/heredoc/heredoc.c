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
		if (!line)
        {
            ft_putstr_fd(
              "minishell: warning: here-document delimited by end-of-file (wanted `",STDERR_FILENO);
            ft_putstr_fd((char *)eof, STDERR_FILENO);
            ft_putstr_fd("')\n", STDERR_FILENO);
            break ;
        }
        if (ft_strcmp(line, eof) == 0)
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
    if (line)
	    free(line);
	return (0);
}

static int process_heredoc(t_command *cmd, const char *heredoc_eof, t_mini *mini)
{
    int pipe_fd[2];
    char *delim;
    int expand;
    pid_t pid;
    int   status;

    if (pipe(pipe_fd) < 0)
    {
        perror("minishell: pipe");
        return (-1);
    }
    expand = check_expand(heredoc_eof);
    delim  = strip_quotes(heredoc_eof);
    pid = fork();
    if (pid < 0)
    {
        perror("minishell: fork");
        free(delim);
        close(pipe_fd[0]); close(pipe_fd[1]);
        return (-1);
    }
    else if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        close(pipe_fd[0]);
        heredoc_readline(delim, expand, pipe_fd[1], mini);
        close(pipe_fd[1]);
        free(delim);
        exit(EXIT_SUCCESS);
    }
    free(delim);
    close(pipe_fd[1]);
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        close(pipe_fd[0]);
        return (1);
    }
    cmd->heredoc_fd = pipe_fd[0];
    return (0);
}

int check_heredocs(t_parse_result *result, t_mini *mini)
{
    t_command *cmd;
    int        i;
    int        j;
    int        err;

    if (!result || !result->commands)
        return (1);
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
                err = process_heredoc(cmd,
                                      cmd->redirs[j].heredoc_eof,
                                      mini);
                if (err == 1)
                {
                    mini->last_status = 130;
                    return (1);
                }
            }
            j++;
        }
        i++;
    }
    return (0);
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
