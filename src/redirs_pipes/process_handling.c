#include "../inc/minishell.h"

void	setup_child_io(int i, int cmd_count, int pipes[2][2], t_command *cmd)
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

void	child_branch(int i, t_parse_result *res, t_mini *mini,
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

void	spawn_commands(t_parse_result *res, t_mini *mini)
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

void wait_processes(pid_t *pids, int n_commands, t_mini *mini)
{
    int i;
    int status;
    int last_status;

	i = 0;
	last_status = 0;
    while (i < n_commands)
    {
        if (waitpid(pids[i], &status, 0) == -1)
            perror("minishell: waitpid");
        else if (WIFEXITED(status))
            last_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            last_status = 128 + WTERMSIG(status);
        i++;
    }
    mini->last_status = last_status;
	if (last_status == 128 + SIGPIPE)
        ft_putstr_fd("Broken pipe\n", STDERR_FILENO);
}

