#include "../inc/minishell.h"

static int *collect_exit_codes(t_parse_result *res, pid_t *pids)
{
    int   *exit_codes;
    int    status;
    int    i;
    int    remaining;
    pid_t  pid;

    exit_codes = alloc_and_zero_exit_codes(res->cmd_count);
    remaining = res->cmd_count;
    while (remaining > 0)
    {
        pid = waitpid(-1, &status, 0);
        if (pid < 0)
            break;
        i = find_pid_index(pids, pid, res->cmd_count);
        if (i == -1)
            continue;
        update_exit_code(exit_codes, i, status);
        remaining--;
    }
    return (exit_codes);
}

static void reap_and_handle_errors(t_parse_result *res,
        t_mini *mini, pid_t *pids)
{
    int  *exit_codes;
    int   last_index;

    last_index = res->cmd_count - 1;
    exit_codes = collect_exit_codes(res, pids);
    print_all_child_errors(res, exit_codes);
    mini->last_status = exit_codes[last_index];
    free(exit_codes);
}

static int	prepare_child_cmd(t_command *cmd, t_mini *mini)
{
	int	redir_failed;
	int	status;
	int	is_path;

	redir_failed = 0;
	if (apply_redirections(cmd))
		redir_failed = 1;
	if (cmd->argv[0] == NULL)
	{
		if (redir_failed)
			exit(1);
		exit(0);
	}
	if (redir_failed)
		exit(1);
	if (is_builtin(cmd->argv[0]))
	{
		status = execute_builtin(cmd, mini);
		exit(status);
	}
	if (ft_strchr(cmd->argv[0], '/'))
		is_path = 1;
	else
		is_path = 0;
	return (is_path);
}

static void	exec_or_exit(char **argv, char **envp, int is_path)
{
	exec_command(argv, envp);
	if (is_path)
	{
		if (errno == EACCES || errno == EISDIR)
			exit(COMMAND_NOT_EXECUTABLE);
		exit(COMMAND_NOT_FOUND);
	}
	exit(COMMAND_NOT_FOUND);
}

void	child_branch(int i, t_parse_result *res, t_mini *mini)
{
	t_command	*cmd;
	int			is_path;

	cmd = &res->commands[i];
	is_path = prepare_child_cmd(cmd, mini);
	exec_or_exit(cmd->argv, mini->our_env, is_path);
}

void spawn_commands(t_parse_result *res, t_mini *mini)
{
    pid_t *pids;
    int    prev_fd = -1;
    int    fd[2];
    int    i;

    pids = alloc_pids(res->cmd_count);
    for (i = 0; i < res->cmd_count; i++)
    {
        if (i < res->cmd_count - 1)
            if (pipe(fd) < 0) { perror("minishell: pipe"); exit(EXIT_FAILURE); }
        pids[i] = fork();
        if (pids[i] < 0)
        {
            perror("minishell: fork");
            exit(EXIT_FAILURE);
        }
        if (pids[i] == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (i < res->cmd_count - 1)
            {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
            }
            if (res->commands[i].heredoc_fd != -1)
            {
                dup2(res->commands[i].heredoc_fd, STDIN_FILENO);
                close(res->commands[i].heredoc_fd);
            }
            child_branch(i, res, mini);
        }
        if (prev_fd != -1)
            close(prev_fd);
        if (i < res->cmd_count - 1)
            close(fd[1]);
        if (i < res->cmd_count - 1)
            prev_fd = fd[0];
        else
            prev_fd = -1;
    }
    reap_and_handle_errors(res, mini, pids);
    dup2(mini->saved_stdin,  STDIN_FILENO);
    dup2(mini->saved_stdout, STDOUT_FILENO);
    free(pids);
}
