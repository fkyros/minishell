#include "../inc/minishell.h"

static void heredoc_readline(const char *heredoc_eof, int pipe_fd[2])
{
    char *line;
    
    while (1)
    {
        line = readline("> ");
        if (!line) 
        {
            ft_putstr_fd("minishell: warning: here-document delimited by EOF\n", STDERR_FILENO);
            break ;
        }
        if (ft_strcmp(line, heredoc_eof) == 0)
        {
            free(line);
            break ;
        }
        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
}

static void process_heredoc(t_command *cmd, const char *heredoc_eof)
{
    int pipe_fd[2];
    
    if (pipe(pipe_fd) == -1) {
        perror(BOLD RED"minishell: pipe"RST);
        return ;
    }
    heredoc_readline(heredoc_eof, pipe_fd);
    close(pipe_fd[1]);
    cmd->heredoc_fd = pipe_fd[0];
}

void check_heredocs(t_parse_result *result)
{
    t_command *cmd;
    int i;
    int j;

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
                process_heredoc(cmd, cmd->redirs[j].heredoc_eof);
                break ;
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