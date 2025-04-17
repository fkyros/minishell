#include "../inc/minishell.h"

void    open_close_pipe(t_parse_result *result, int *i, int (*pipe_fd)[2])
{
    if (!result->commands[*i].is_last)
        {
            if (pipe(*pipe_fd) < 0)
            {
                perror(RED BOLD"pipe"RST);
                exit(EXIT_FAILURE);
            }
        }
    else
        {
            (*pipe_fd)[0] = -1;
            (*pipe_fd)[1] = -1;
        }
}

void setup_pipes_and_redirection(t_command *cmd, int prev_pipe_fd, int (*pipe_fd)[2])
{
    setup_input(cmd, prev_pipe_fd);
    setup_output(cmd, pipe_fd);
}

void execute_pipeline(t_parse_result *result, t_mini *mini)
{
    int i;
    int prev_pipe_fd = -1;
    int pipe_fd[2];
    pid_t pid;

    check_heredocs(result);

    if (result->cmd_count == 1 && is_builtin(result->commands[0].argv[0]))
    {
        execute_builtin(&result->commands[0], 1, mini);
        return ;
    }
    i = 0;
    while (i < result->cmd_count)
    {
        if (is_builtin(result->commands[i].argv[0]))
        {
            open_close_pipe(result, &i, &pipe_fd);
            pid = fork();
            if (pid == 0)
                child_process(result, &i, &pipe_fd, &prev_pipe_fd, mini);
            else if (pid > 0)
                parent_process(result, &i, &pipe_fd, &prev_pipe_fd);
            else
                perror("minishell: fork");
        }
        else
        {
            open_close_pipe(result, &i, &pipe_fd);
            pid = fork();
            process_handling(&pid, result, &i, &pipe_fd, &prev_pipe_fd, mini);
        }
        i++;
    }
    wait_processes(result);
    close_heredocs(result);
}