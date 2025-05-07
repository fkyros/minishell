#include "../inc/minishell.h"

void    open_close_pipe(t_parse_result *result, int *i, int (*pipe_fd)[2])
{
    if (!result->commands[*i].is_last)
        {
            if (pipe(*pipe_fd) < 0)
            {
                perror(RED BOLD"minishell: pipe"RST);
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
    pid_t *pids;
    
    pids = malloc(sizeof(pid_t) * result->cmd_count);
    if (!pids)
    {
        perror("minishell: malloc");
        return ;
    }
    check_heredocs(result, mini);
    i = 0;
    while (i < result->cmd_count)
    {
        open_close_pipe(result, &i, &pipe_fd);
        pid = fork();
        if (pid == 0) 
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            child_process(result, &i, &pipe_fd, &prev_pipe_fd, mini);
        }
        else if (pid > 0)
        {
            pids[i] = pid;
           parent_process(result, &i, &pipe_fd, &prev_pipe_fd);
        }
        else
            perror(BOLD RED"minishell: fork"RST);
        i++;
    }
    wait_processes(pids, result->cmd_count, mini);
    free(pids);
    close_heredocs(result);
}