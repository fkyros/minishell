#include "../../../inc/minishell.h"

void	print_heredoc_warning(const char *eof)
{
	ft_putstr_fd("minishell: warning: here-document ", STDERR_FILENO);
	ft_putstr_fd("delimited by end-of-file (wanted '", STDERR_FILENO);
	ft_putstr_fd((char *)eof, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

void	write_heredoc_line(char *line, int expand, int fd, t_mini *mini)
{
	char	*tmp;

	if (expand)
	{
		tmp = line;
		line = expand_line(tmp, mini);
		free(tmp);
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

int	heredoc_fork_error(char *delim, int fd0, int fd1)
{
	perror("minishell: fork");
	free(delim);
	close(fd0);
	close(fd1);
	return (-1);
}

void	heredoc_child_process(char *delim,
	int expand, int write_fd, t_mini *mini)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	heredoc_readline(delim, expand, write_fd, mini);
	close(write_fd);
	free(delim);
	exit(EXIT_SUCCESS);
}

int	heredoc_parent_after(pid_t pid, int read_fd, t_command *cmd)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(read_fd);
		return (1);
	}
	cmd->heredoc_fd = read_fd;
	return (0);
}
