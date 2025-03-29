#include "../inc/minishell.h"

// < (input), << (heredoc), > (output), >> (output append)

static int	handle_input(char **args, int i)
{
	int	fd;

	fd = open(args[i + 1], O_RDONLY);
	if (fd < 0)
	{
		perror("Minishell: error trying to open file");
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	args[i] = NULL;
	return (0);
}

static int	handle_heredoc(char **args, int i)
{
	int	pipe_fd[2];
	char	*line;
	size_t	len;

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
	return (0);
}

static int	handle_output(char **args, int i)
{
	int	fd;

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
	return (0);
}

static int	handle_append(char **args, int i)
{
	int	fd;

	fd = open(args[i + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("Minishell: error trying to open file");
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	args[i] = NULL;
	return (0);
}

int	parse_redirections(char **args)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "<") == 0)
			status = handle_input(args, i);
		else if (ft_strcmp(args[i], "<<") == 0)
			status = handle_heredoc(args, i);
		else if (ft_strcmp(args[i], ">") == 0)
			status = handle_output(args, i);
		else if (ft_strcmp(args[i], ">>") == 0)
			status = handle_append(args, i);
		else if (ft_strcmp(args[i], "|") == 0)
			status = handle_pipe(args, i);
		if (status == -1)
			return (-1);
		if (args[i] == NULL)
			i = 0;
		else
			i++;
	}
	return (0);
}
