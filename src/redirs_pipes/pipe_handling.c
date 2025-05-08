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

static void	handle_single_builtin(t_parse_result *result, t_mini *mini)
{
	if (result->cmd_count == 1
		&& is_builtin(result->commands[0].argv[0]))
	{
		execute_builtin(&result->commands[0], mini);
		close_heredocs(result);
	}
}

static pid_t	*alloc_pids(int count)
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

static void	create_pipe(int i, int cmd_count, int pipes[2][2])
{
	if (i < cmd_count - 1 && pipe(pipes[i % 2]) < 0)
	{
		perror("minishell: pipe");
		exit(EXIT_FAILURE);
	}
}

static void	setup_child_io(int i, int cmd_count, int pipes[2][2], t_command *cmd)
{
	if (i > 0)
	{
		dup2(pipes[(i - 1) % 2][0], STDIN_FILENO);
		close(pipes[(i - 1) % 2][1]);
	}
	if (cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	if (i < cmd_count - 1)
	{
		dup2(pipes[i % 2][1], STDOUT_FILENO);
		close(pipes[i % 2][0]);
	}
}

static void	child_branch(int i, t_parse_result *res, t_mini *mini,
	int pipes[2][2])
{
	t_command *cmd = &res->commands[i];

	setup_child_io(i, res->cmd_count, pipes, cmd);
	if (apply_redirections(cmd))
		exit(EXIT_FAILURE);
	if (is_builtin(cmd->argv[0]))
	{
		execute_builtin(cmd, mini);
		exit(mini->last_status);
	}
	exec_command(cmd->argv, mini->our_env);
	perror(cmd->argv[0]);
	exit(127);
}

static void	spawn_commands(t_parse_result *res, t_mini *mini)
{
	int		i;
	int		pipes[2][2];
	pid_t	*pids;

	pids = alloc_pids(res->cmd_count);
	i = 0;
	while (i < res->cmd_count)
	{
		create_pipe(i, res->cmd_count, pipes);
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("minishell: fork");
			exit(EXIT_FAILURE);
		}
		if (pids[i] == 0)
			child_branch(i, res, mini, pipes);
		if (i > 0)
		{
			close(pipes[(i - 1) % 2][0]);
			close(pipes[(i - 1) % 2][1]);
		}
		i++;
	}
	wait_processes(pids, res->cmd_count, mini);
	free(pids);
}

void	execute_pipeline(t_parse_result *result, t_mini *mini)
{
	check_heredocs(result, mini);
	handle_single_builtin(result, mini);
	if (result->cmd_count == 1
		&& is_builtin(result->commands[0].argv[0]))
		return ;
	spawn_commands(result, mini);
	close_heredocs(result);
}



