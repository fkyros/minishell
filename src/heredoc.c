#include "../inc/minishell.h"

static void heredoc_readline(t_command *cmd, int (*pipe_fd)[2])
{
    char    *line;

    while (1)
    {
        line = readline("> ");
        if (!line) 
        {
            ft_putstr_fd(RED BOLD"minishell: HEREDOC delimited by EOF\n"RST, STDERR_FILENO);
            break ;
        }
        if (ft_strcmp(line, cmd->heredoc) == 0)
        {
            free(line);
            break ;
        }
        write((*pipe_fd)[1], line, ft_strlen(line));
        write((*pipe_fd)[1], "\n", 1);
        free(line);
    }
}

static int process_heredoc(t_command *cmd) {
    int pipe_fd[2];

    if (!cmd->heredoc) 
        return (0);
    if (pipe(pipe_fd)) 
    {
        perror(RED BOLD"minishell: pipe"RST);
        return (-1);
    }
    heredoc_readline(cmd, &pipe_fd);
    close(pipe_fd[1]);
    cmd->heredoc_fd = pipe_fd[0];
    return (0);
}

void check_heredoc(t_parse_result *result)
{
    int i;

    i = 0;
    while (i < result->cmd_count)
        {
            if (process_heredoc(&result->commands[i]) == -1)
            {
                while (i < result->cmd_count)
                {
                    if (result->commands[i].heredoc_fd != -1)
                        close(result->commands[i].heredoc_fd);
                    i++;
                }
                return ;
            }
            i++;
        }
}

void    close_heredocs(t_parse_result *result)
{
    int i;
    
    i = 0;
    while (i < result->cmd_count) 
    {
        if (result->commands[i].heredoc_fd != -1)
            close(result->commands[i].heredoc_fd);
        i++;
    }
}