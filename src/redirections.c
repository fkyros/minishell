#include "minishell.h"

// < (input), << (heredoc), > (output), >> (output append)

static int	handle_input(char **args, int i)
{
	int	fd;
	int	stdin_copy;

	fd = open(args[i + 1], O_RDONLY);
	stdin_copy = dup(STDIN_FILENO);
	if (fd < 0)
	{
		perror("Minishell: error trying to open file");
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	args[i] = NULL;
	dup2(stdin_copy, STDIN_FILENO);
	close(stdin_copy);
	return (0);
}

static int	handle_heredoc(char **args, int i)
{
	int	pipe_fd[2];
	int	stdin_copy;
	char	*line;
	size_t	len;

	stdin_copy = dup(STDIN_FILENO);
	if (pipe(pipe_fd) == -1)
	{
		perror("Minishell: error trying to create a pipe (heredoc)");
		return (-1);
	}
	printf("Minishell: waiting for heredoc input (Ctrl+D to exit)...\n");
	line = NULL;
	len = 0;
	while (getline(&line, &len, stdin) != -1)
	{
		if (ft_strcmp(line, args[i + 1]) == 0)
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
	}
	free(line);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	args[i] = NULL;
	dup2(stdin_copy, STDIN_FILENO);
	close(stdin_copy);
	return (0);
}

static int	handle_output(char **args, int i)
{
	int	fd;
	int	stdout_copy;

	stdout_copy = dup(STDOUT_FILENO);
	fd = open(args[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("Minishell: error trying to open file");
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	args[i] = NULL;
	args[i + 1] = NULL;
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdout_copy);
	return (0);
}

static int	handle_append(char **args, int i)
{
	int	fd;
	int	stdout_copy;

	fd = open(args[i + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	stdout_copy = dup(STDOUT_FILENO);
	if (fd < 0)
	{
		perror("Minishell: error trying to open file");
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	args[i] = NULL;
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdout_copy);
	return (0);
}

int	parse_redirections(char **args)
{
	int	i = 0;

	while (args[i])
	{
		if (ft_strcmp(args[i], "<") == 0)
			return (handle_input(args, i));
		else if (ft_strcmp(args[i], "<<") == 0)
			return (handle_heredoc(args, i));
		else if (ft_strcmp(args[i], ">") == 0)
			return (handle_output(args, i));
		else if (ft_strcmp(args[i], ">>") == 0)
			return (handle_append(args, i));
		else if (ft_strcmp(args[i], "|") == 0)
			return (handle_pipe(args, i));
		i++;
	}
	return (0);
}
