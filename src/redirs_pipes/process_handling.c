#include "../inc/minishell.h"

void setup_child_io(int i, int cmd_count, int pipes[2][2], t_command *cmd)
{
    if (i > 0)
        dup2(pipes[(i - 1) % 2][0], STDIN_FILENO);
    if (i < cmd_count - 1)
        dup2(pipes[i % 2][1], STDOUT_FILENO);
    close(pipes[0][0]);
    close(pipes[0][1]);
    close(pipes[1][0]);
    close(pipes[1][1]);
    if (cmd->heredoc_fd != -1)
    {
        dup2(cmd->heredoc_fd, STDIN_FILENO);
        close(cmd->heredoc_fd);
    }
}

void child_branch(int i, t_parse_result *res, t_mini *mini,
                  int pipes[2][2])
{
    t_command *cmd;
    int        is_path;

    cmd = &res->commands[i];
    is_path = (ft_strchr(cmd->argv[0], '/') != NULL);
    setup_child_io(i, res->cmd_count, pipes, cmd);
    if (apply_redirections(cmd))
        exit(1);
    exec_command(cmd->argv, mini->our_env);
    if (is_path)
    {
        if (errno == EACCES || errno == EISDIR)
            exit(126);
        else
            exit(127);
    }
    else
        exit(127);
}

static void	reap_and_handle_errors(t_parse_result *res,
	t_mini *mini, pid_t *pids)
{
	int		status;
	int		i;
	int		remaining;
	pid_t	pid;
	int		*exit_codes;

	remaining = res->cmd_count;
	exit_codes = alloc_and_zero_exit_codes(res->cmd_count);
	while (remaining > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid < 0)
			break ;
		i = find_pid_index(pids, pid, res->cmd_count);
		if (i == -1)
			continue ;
		update_exit_code(exit_codes, i, status);
		remaining--;
	}
	print_all_child_errors(res, exit_codes);
	mini->last_status = exit_codes[res->cmd_count - 1];
	free(exit_codes);
}

void spawn_commands(t_parse_result *res, t_mini *mini)
{
    pid_t *pids;
    int    pipes[2][2];
    int    i;

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
    if (res->cmd_count > 1)
    {
        close(pipes[(i - 1) % 2][0]);
        close(pipes[(i - 1) % 2][1]);
    }
    reap_and_handle_errors(res, mini, pids);
    dup2(mini->saved_stdin, STDIN_FILENO);
    free(pids);
}
