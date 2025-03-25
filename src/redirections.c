#include "minishell.h"

// < (input), << (heredoc), > (output), >> (output append)

void	handle_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("Input redirection failed!");
		exit(1);
	}
	dup2(fd, 0);
	close(fd);
}

void	handle_heredoc(char *delim)
{
	char	*line;
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
}

void	handle_output(char *filename)
{}
