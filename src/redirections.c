#include "../inc/minishell.h"

void    redirect_input_heredoc(t_command *cmd)
{
    int fd;

    // REDIRECT HEREDOC
    if (cmd->heredoc_fd != -1) 
    {
        dup2(cmd->heredoc_fd, STDIN_FILENO);
        close(cmd->heredoc_fd);
        cmd->heredoc_fd = -1;
    }
    // REDIRECT INPUT
    else if (cmd->redirect_in)
    {
        fd = open(cmd->redirect_in, O_RDONLY);
        if (fd == -1)
        {
            perror("open");
            return ;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
}

void    redirect_output(t_command *cmd)
{
    int fd;

    if (cmd->redirect_out)
    {
        fd = open(cmd->redirect_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror("open");
            return ;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

// FUNCTION THAT APPLIES REDIRECTIONS TO BUILTINS
int apply_redirections(t_command *cmd)
{
    int fd_append;

    // REDIRECT HEREDOC AND INPUT
    redirect_input_heredoc(cmd);
    // REDIRECT OUTPUT
    redirect_output(cmd);
    // REDIRECT APPEND
    if (cmd->redirect_append)
    {
        fd_append = open(cmd->redirect_append, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd_append == -1)
        {
            perror("open");
            return (-1);
        }
        dup2(fd_append, STDOUT_FILENO);
        close(fd_append);
    }
    return (0);
}

/*	CONFIGURING PROCESS INPUT. IF THERE'S AN EXPLICIT REDIRECTION WE'LL OPEN THE FILE
	IF IT'S NOT THE FIRST COMMAND, WE DUP LAST COMMAND PIPE'S READING END */

void setup_input(t_command *cmd, int prev_pipe_fd)
{
    int fd;

    if (cmd->heredoc_fd != -1) {
        dup2(cmd->heredoc_fd, STDIN_FILENO);
        close(cmd->heredoc_fd);
        cmd->heredoc_fd = -1;
    }
    else if (cmd->redirect_in)
    {
        fd = open(cmd->redirect_in, O_RDONLY);
        if (fd < 0)
        {
            perror("open redirect_in");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
	/*	IF IT'S NOT THE FIRST COMMAND, AND THERE ISN'T AN EXPLICIT REDIRECTION
		WE USE LAST COMMAND PIPE'S READING END */
    else if (!cmd->is_first)
        dup2(prev_pipe_fd, STDIN_FILENO);
}
/*	CONFIGURING PROCESS OUTPUT. IF THERE'S AN EXPLICIT REDIRECTION WE'LL OPEN THE FILE
	IF IT HAS TO BE SENT THROUGH A PIPE, WE DUP THE CURRENT PIPE'S WRITING END */

void setup_output(t_command *cmd, int pipe_fd[2])
{
    int fd;
	
	if (cmd->redirect_out || cmd->redirect_append)
	{
		if (cmd->redirect_out)
			fd = open(cmd->redirect_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(cmd->redirect_append, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
        {
            perror("open redirect_out");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	/*	IF THE INFO HAS TO BE SENT THROUGH A PIPE,
		WE DUP THE CURRENT PIPE'S WRITING END TO STDOUT*/
    else if (cmd->pipe_out)		
        dup2(pipe_fd[1], STDOUT_FILENO);
}
