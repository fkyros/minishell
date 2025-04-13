#include "../inc/minishell.h"

// FUNCTION THAT APPLIES REDIRECTIONS TO BUILTINS
int apply_redirections(t_command *cmd)
{
    int fd_in;
    int fd_out;

    // REDIRECT INPUT
    if (cmd->heredoc_fd != -1) 
    {
        dup2(cmd->heredoc_fd, STDIN_FILENO);
        close(cmd->heredoc_fd);
        cmd->heredoc_fd = -1;
    }
    else if (cmd->redirect_in)
    {
        fd_in = open(cmd->redirect_in, O_RDONLY);
        if (fd_in == -1)
        {
            perror("open");
            return (-1);
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    // REDIRECT OUTPUT
    if (cmd->redirect_out)
    {
        fd_out = open(cmd->redirect_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out == -1)
        {
            perror("open");
            return (-1);
        }
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }

    // REDIRECT APPEND
    if (cmd->redirect_append)
    {
        fd_out = open(cmd->redirect_append, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd_out == -1)
        {
            perror("open");
            return (-1);
        }
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
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

/*	FUNCTION THAT GROUPS UP I/O CONFIGURATION FOR A COMMAND
		- prev_pipe_fd: LAST PIPE'S READING END (if there is one)
		- pipe_fd: int[2] SO THAT THE PIPE CONNECTS TO THE NEXT COMMAND	*/
void setup_pipes_and_redirection(t_command *cmd, int prev_pipe_fd, int pipe_fd[2])
{
    setup_input(cmd, prev_pipe_fd);
    setup_output(cmd, pipe_fd);
}

void execute_pipeline(t_parse_result *result, char **our_env)
{
    int i;
    int prev_pipe_fd = -1;
    int pipe_fd[2];
    pid_t pid;
	char	*path;

    i = 0;
    // CHECK IF CMD DOESN'T EXIST AND ISN'T A BUILTIN (maybe needs to return some kind of error value and print in STDERR..?)
    while (i < result->cmd_count)
    {
        path = search_command(result->commands[i].argv[0], our_env);
	    if ((!path || ft_strcmp(path, "/") == 0) && !is_builtin(result->commands[i].argv[0]))
        {
            print_path_error(path, result, i);
            free(path);
            return ;
        }
        free(path);
        i++;
    }
    i = 0;
    while (i < result->cmd_count)
    {
    if (process_heredoc(&result->commands[i]) == -1)
    {
        while (i < result->cmd_count)
        {
            if (result->commands[i].heredoc_fd != -1)
                close(result->commands[i].heredoc_fd);
            i++;
        }
        return ;
    }
    i++;
    }
    i = 0;
	while (i < result->cmd_count)
    {
        if (is_builtin(result->commands[i].argv[0]))
        {
            execute_builtin(&result->commands[i], our_env);
            i++;
            continue;
        }
		// IF IT'S NOT THE LAST COMMAND, WE CREATE A PIPE
        if (!result->commands[i].is_last)
        {
            if (pipe(pipe_fd) < 0)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
			// DON'T NEED AN OUTGOING PIPE ON THE LAST COMMAND
            pipe_fd[0] = -1;
            pipe_fd[1] = -1;
        }
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)  // CHILD PROCESS
        {
			/* CONFIGURING REDIRECTIONS
				- IF IT'S NOT THE FIRST CMD, IT USES LAST PIPE'S READING END
				- IF IT'S NOT THE LAST AND IT HAS TO SEND INFO THROUGH A PIPE, IT'LL USE THE CURRENT PIPE'S WRITING END
			*/
            setup_pipes_and_redirection(&result->commands[i], prev_pipe_fd, pipe_fd);
            
            // CLOSING UNNECESSARY FDs
            if (prev_pipe_fd != -1)
                close(prev_pipe_fd);
            if (pipe_fd[0] != -1)
                close(pipe_fd[0]);
            if (pipe_fd[1] != -1)
                close(pipe_fd[1]);
            // NORMAL CMD EXEC
            path = search_command(result->commands[i].argv[0], our_env);
            execve(path, result->commands[i].argv, our_env);
            perror("execve");
            free(path);
            exit(EXIT_FAILURE);
        }
        else  // PARENT PROCESS
        {
            if (result->commands[i].heredoc_fd != -1) {
                close(result->commands[i].heredoc_fd);
                result->commands[i].heredoc_fd = -1;
            }
			// PARENT CLOSES WRITING END OF THE CURRENT PIPE, SINCE IT WILL ONLY BE USED IN THE NEXT CMD
            if (pipe_fd[1] != -1)
                close(pipe_fd[1]);
			// CURRENT READING END OF THE PIPE WILL BE PASSED ONTO THE NEXT CMD AS prev_pipe_fd
            if (prev_pipe_fd != -1)
                close(prev_pipe_fd);
            prev_pipe_fd = pipe_fd[0];
        }
		i++;
    }
    // WAITING FOR ALL CHILD PROCESSES TO FINISH
	i = 0;
	while (i < result->cmd_count)
	{
		wait(NULL);
		i++;		
	}
    i = 0;
    while (i < result->cmd_count) {
        if (result->commands[i].heredoc_fd != -1)
            close(result->commands[i].heredoc_fd);
        i++;
    }
}