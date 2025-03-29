#include "../inc/minishell.h"

pid_t	g_child_pid = 0;

void	signal_handling(int sig)
{
	(void)sig;
	char *cwd = get_cwd();

	if (g_child_pid > 0)
		kill(g_child_pid, SIGINT);
	else
	{
		printf("%s > ", cwd);
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	free(cwd);
}

static void	exec(char **cmd, char **env)
{
	char	*path;

	if (!cmd || !cmd[0])
	{
		ft_putendl_fd("Minishell: invalid command!: ", 2);
		exit(COMMAND_NOT_FOUND);
	}
	path = search_command(cmd[0], env);
	if (!path)
	{
		ft_putstr_fd("Minishell: command not found!: ", 2);
		ft_putendl_fd(cmd[0], 2);
		free_split(cmd);
		exit(COMMAND_NOT_FOUND);
	}
	execve(path, cmd, env);
	perror("Minishell: error");
	free(path);
	free_split(cmd);
	exit(COMMAND_NOT_EXECUTABLE);
}

void	shell_exec(char **args, char **env)
{
	pid_t	pid;
	int	status;
	
	if (!args || !args[0])
		return ;
	pid = fork();
	if (pid < 0)
	{
		perror("Minishell: forking failed");
		return ;
	}
	else if (pid == 0)
		exec(args, env);
	else
	{
		g_child_pid = pid;
		waitpid(pid, &status, 0);
		g_child_pid = 0;
		if (status != 0)
			printf("Minishell: finished process with status %d\n", status);
	}
}

void	shell_start(char **args, char **env)
{

	// TESTING ARGS

        for (int z = 0; args[z]; z++) {
                printf("ARG %d: %s\n", z, args[z]);
        }
        // -------------------
	int	i;
	int	original_stdin;
	int	original_stdout;
	int	redir_status;
	int	builtin_executed = 0;

	i = 0;
	if (!args || !args[i])
		return ;
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdin == -1 || original_stdout == -1)
	{
		perror("Minishell: Error saving original descriptors");
		return ;
	}
	redir_status = parse_redirections(args);
	if (redir_status == -1)
	{
		dup2(original_stdin, STDIN_FILENO);
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdin);
		close(original_stdout);
		return ;
	}
	while (args[i] && !builtin_executed)
	{
		if (ft_strcmp(args[i], "echo") == 0)
		{
			builtin_echo(args);
			builtin_executed = 1;
		}
		else if (ft_strcmp(args[i], "cd") == 0)
		{
			builtin_cd(args);
			builtin_executed = 1;
		}
		else if (ft_strcmp(args[i], "pwd") == 0)
		{
			builtin_pwd();
			builtin_executed = 1;
		}
		else if (ft_strcmp(args[i], "env") == 0)
		{
			builtin_env(env);
			builtin_executed = 1;
		}
		else if (ft_strcmp(args[i], "exit") == 0)
			builtin_exit(args);
		i++;
	}
	if (!builtin_executed)
		shell_exec(args, env);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

int	main(int argc, char **argv, char **env)
{
	//REPL
	
	char	*cwd;
	char	*line;
	char	**args;

	(void)argc;
	(void)argv;

	signal(SIGINT, signal_handling);
	signal(SIGQUIT, SIG_IGN);
	cwd = get_cwd();
	printf("%s > ", cwd);
	free(cwd);
	while (1)
	{
		line = readline("funny shell > ");
		if (!line)
		{
			printf("Minishell: exiting!\n");
			break;
		}
		if (*line)
		{
			add_history(line);
			args = parse_command(line);
			shell_start(args, env); //Already contains builtins
			free_split(args);
		}
		cwd = get_cwd();
		printf("%s > ", cwd);
		free(line);
		free(cwd);
	}
	return (EXIT_SUCCESS);
}
