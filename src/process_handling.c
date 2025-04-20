#include "../inc/minishell.h"

void child_process(t_parse_result *result, int *i, int (*pipe_fd)[2],
                  int *prev_pipe_fd, t_mini *mini)
{
	t_command	*cmd;
	char		*path;

	cmd = &result->commands[*i];
	if (!cmd->argv[0])
	{
		if (cmd->redir_count > 0)
		{
			setup_pipes_and_redirection(cmd, *prev_pipe_fd, pipe_fd);
			exit(0);
		}
		else
		{
			ft_putstr_fd("minishell: syntax error: empty command\n", STDERR_FILENO);
			exit(1);
		}
	}
	if (cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	setup_pipes_and_redirection(cmd, *prev_pipe_fd, pipe_fd);
	if (*prev_pipe_fd != -1)
		close(*prev_pipe_fd);
	if ((*pipe_fd)[0] != -1)
		close((*pipe_fd)[0]);
	if ((*pipe_fd)[1] != -1)
		close((*pipe_fd)[1]);
	if (is_builtin(cmd->argv[0]))
	{
		execute_builtin(cmd, 0, mini);
		free_array(mini->our_env);
		free(mini);
		exit(0);
	}
	else
	{
		path = search_command(cmd->argv[0], mini->our_env);
		if (path)
		{
			execve(path, cmd->argv, mini->our_env);
			free(path);
		}
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		free_commands(result);
		free_array(mini->our_env);
		free(mini);
		exit(127);
	}
}

void	parent_process(t_parse_result *result, int *i, int (*pipe_fd)[2],
	int *prev_pipe_fd)
{
	if (result->commands[*i].heredoc_fd != -1)
	{
		close(result->commands[*i].heredoc_fd);
		result->commands[*i].heredoc_fd = -1;
	}
	if ((*pipe_fd)[1] != -1)
		close((*pipe_fd)[1]);
	if (*prev_pipe_fd != -1)
		close(*prev_pipe_fd);
	*prev_pipe_fd = (*pipe_fd)[0];
}

void process_handling(int *pid, t_parse_result *result, int *i,
    int (*pipe_fd)[2], int *prev_pipe_fd, t_mini *mini)
{
	if (*pid < 0)
	{
		perror(RED BOLD"Error trying to fork"RST);
		exit(EXIT_FAILURE);
	}
	else if (*pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		child_process(result, i, pipe_fd, prev_pipe_fd, mini);
	}
	else
		parent_process(result, i, pipe_fd, prev_pipe_fd);
}

void wait_processes(t_parse_result *result, t_mini *mini)
{
    int i;
    int status;
    int last_status;

	i = 0;
	last_status = 0;
    while (i < result->cmd_count)
    {
        if (wait(&status) == -1)
            perror(BOLD RED"minishell: wait"RST);
        else
        {
            if (WIFEXITED(status))
                last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                last_status = 128 + WTERMSIG(status);
        }
        i++;
    }
    mini->last_status = last_status;
}
