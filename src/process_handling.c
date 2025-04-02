#include "../inc/minishell.h"

void    child_process(t_parse_result *result, int *i, int (*pipe_fd)[2], 
    int *prev_pipe_fd, char **env)
{
			/* CONFIGURING REDIRECTIONS
				- IF IT'S NOT THE FIRST CMD, IT USES LAST PIPE'S READING END
				- IF IT'S NOT THE LAST AND IT HAS TO SEND INFO THROUGH A PIPE, IT'LL USE THE CURRENT PIPE'S WRITING END
			*/
    char    *path;
        
    setup_pipes_and_redirection(&result->commands[*i], *prev_pipe_fd, *pipe_fd);   
    // CLOSING UNNECESSARY FDs
    if (*prev_pipe_fd != -1)
        close(*prev_pipe_fd);
    if ((*pipe_fd)[0] != -1)
        close((*pipe_fd)[0]);
    if ((*pipe_fd)[1] != -1)
        close((*pipe_fd)[1]);
    // NORMAL CMD EXEC
    path = search_command(result->commands[*i].argv[0], env);
    execve(path, result->commands[*i].argv, env);
    perror("execve");
    free(path);
    exit(EXIT_FAILURE);
}

void    parent_process(t_parse_result *result, int *i, int (*pipe_fd)[2],
    int *prev_pipe_fd)
{
    if (result->commands[*i].heredoc_fd != -1)
    {
        close(result->commands[*i].heredoc_fd);
        result->commands[*i].heredoc_fd = -1;
    }
	// PARENT CLOSES WRITING END OF THE CURRENT PIPE, SINCE IT WILL ONLY BE USED IN THE NEXT CMD
    if ((*pipe_fd)[1] != -1)
        close((*pipe_fd)[1]);
    // CURRENT READING END OF THE PIPE WILL BE PASSED ONTO THE NEXT CMD AS prev_pipe_fd
    if (*prev_pipe_fd != -1)
        close(*prev_pipe_fd);
    *prev_pipe_fd = (*pipe_fd)[0];
}

void    process_handling(int *pid, t_parse_result *result, int *i, 
    int (*pipe_fd)[2], int *prev_pipe_fd, char **env)
{
    // FORK ERROR
    if (*pid < 0)
        {
            perror(RED BOLD"Error trying to fork"RST);
            exit(EXIT_FAILURE);
        }
        // CHILDREN
    else if (*pid == 0)
        child_process(result, i, pipe_fd, prev_pipe_fd, env);
        // PARENT
    else
        parent_process(result, i, pipe_fd, prev_pipe_fd);
}

void    wait_processes(t_parse_result *result)
{
    int i;

    i = 0;
	while (i < result->cmd_count)
	{
		wait(NULL);
		i++;		
	}
}