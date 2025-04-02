#include "../inc/minishell.h"

// CHECK IF CMD DOESN'T EXIST AND ISN'T A BUILTIN (maybe needs to return some kind of error value and print in STDERR..?)
void cmd_exists(t_parse_result *result, char **env)
{
    int i;
    char    *path;

    i = 0;
    while (i < result->cmd_count)
        {
            path = search_command(result->commands[i].argv[0], env);
            if ((!path || ft_strcmp(path, "/") == 0) && !is_builtin(result->commands[i].argv[0]))
            {
                print_path_error(path, result, i);
                free(path);
                return ;
            }
            free(path);
            i++;
        }
}

void    open_close_pipe(t_parse_result *result, int *i, int (*pipe_fd)[2])
{
    if (!result->commands[*i].is_last)
        {
            if (pipe(*pipe_fd) < 0)
            {
                perror(RED BOLD"pipe"RST);
                exit(EXIT_FAILURE);
            }
        }
    else
        {
			// DON'T NEED AN OUTGOING PIPE ON THE LAST COMMAND
            (*pipe_fd)[0] = -1;
            (*pipe_fd)[1] = -1;
        }
}

/*	FUNCTION THAT GROUPS UP I/O CONFIGURATION FOR A COMMAND
		- prev_pipe_fd: LAST PIPE'S READING END (if there is one)
		- pipe_fd: int[2] SO THAT THE PIPE CONNECTS TO THE NEXT COMMAND	*/
void setup_pipes_and_redirection(t_command *cmd, int prev_pipe_fd, int pipe_fd[2])
{
    setup_input(cmd, prev_pipe_fd);
    setup_output(cmd, pipe_fd);
}

void execute_pipeline(t_parse_result *result, char **env)
{
    int i;
    int prev_pipe_fd = -1;
    int pipe_fd[2];
    pid_t pid;

    cmd_exists(result, env);
    check_heredoc(result);
    i = 0;
	while (i < result->cmd_count)
    {
        if (is_builtin(result->commands[i].argv[0]))
        {
            execute_builtin(&result->commands[i]);
            i++;
            continue ;
        }
		// IF IT'S NOT THE LAST COMMAND, WE CREATE A PIPE
        open_close_pipe(result, &i, &pipe_fd);
        pid = fork();
        process_handling(&pid, result, &i, &pipe_fd, &prev_pipe_fd, env);
        i++;
    }
    // WAITING FOR ALL CHILD PROCESSES TO FINISH
	wait_processes(result);
    close_heredocs(result);
}