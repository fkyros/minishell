#include "minishell.h"

static int	create_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
	{
		perror("Minishell: error trying to create a pipe (pipe handling)");
		return (-1);
	}
	return (0);
}

static int	execute_pipe(char **args, int i, int pipe_fds[2])
{
	pid_t pid;
	int	stdin_copy;
	int	stdout_copy;

	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	pid = fork();
	if (pid < 0)
	{
		perror("Minishell: error trying to fork (pipe handling)");
		return (-1);
	}
	if (pid == 0)
	{
		close(pipe_fds[0]);
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
		args[i] = NULL;
	}
	else
	{
		close(pipe_fds[1]);
		dup2(pipe_fds[0], STDIN_FILENO);
		close(pipe_fds[0]);
		waitpid(pid, NULL, 0);
	}
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
	return (0);
}

int	handle_pipe(char **args, int i)
{
	int	pipe_fds[2];
	if (create_pipe(pipe_fds) == -1)
		return (-1);
	return (execute_pipe(args, i, pipe_fds));
}
