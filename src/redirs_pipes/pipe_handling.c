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
    int            i;
    int            pipes[2][2];
    pid_t          *pids;

    check_heredocs(result, mini);
    pids = malloc(sizeof(pid_t) * result->cmd_count);
    if (!pids)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    i = 0;
    while (i < result->cmd_count)
    {
        if (i < result->cmd_count - 1 && pipe(pipes[i % 2]) < 0)
            perror("minishell: pipe"), exit(1);
        pids[i] = fork();
        if (pids[i] < 0)
            perror("minishell: fork"), exit(1);
        if (pids[i] == 0)
        {
            if (i > 0)
            {
                dup2(pipes[(i-1) % 2][0], STDIN_FILENO);
                close(pipes[(i-1) % 2][1]);
            }
            if (i < result->cmd_count - 1)
            {
                dup2(pipes[i % 2][1], STDOUT_FILENO);
                close(pipes[i % 2][0]);
            }
            if (apply_redirections(&result->commands[i]))
                exit(1);
            if (is_builtin(result->commands[i].argv[0]))
                execute_builtin(&result->commands[i], mini), exit(mini->last_status);
            else
                exec_command(result->commands[i].argv, mini->our_env);
        }
        if (i > 0)
        {
            close(pipes[(i-1) % 2][0]);
            close(pipes[(i-1) % 2][1]);
        }
        i++;
    }
    i = 0;
    wait_processes(pids, result->cmd_count, mini);
    free(pids);
}
