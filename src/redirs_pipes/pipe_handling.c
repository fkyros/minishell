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

pid_t	*alloc_pids(int count)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * count);
	if (!pids)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (pids);
}

void	create_pipe(int i, int cmd_count, int pipes[2][2])
{
	if (i < cmd_count - 1 && pipe(pipes[i % 2]) < 0)
	{
		perror("minishell: pipe");
		exit(EXIT_FAILURE);
	}
}

void execute_pipeline(t_parse_result *result, t_mini *mini)
{
    check_heredocs(result, mini);
    if (mini->last_status == 130)
        return;
    spawn_commands(result, mini);
    close_heredocs(result);
}



