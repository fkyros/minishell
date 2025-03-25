#include "minishell.h"

pid_t	g_child_pid = 0;

char	*get_cwd(void)
{
	char	*cwd = malloc(2048);
	if (!cwd)
	{
		perror("Error trying to assign memory for the current directory");
		exit(1);
	}
	if (!getcwd(cwd, 2048))
	{
		perror("Error trying to obtain current directory");
		free(cwd);
		exit(1);
	}
	return (cwd);
}

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
		exit(127);
	}
	path = search_command(cmd[0], env);
	if (!path)
	{
		ft_putstr_fd("Minishell: command not found!: ", 2);
		ft_putendl_fd(cmd[0], 2);
		free_split(cmd);
		exit(127);
	}
	execve(path, cmd, env);
	perror("Minishell: error");
	free(path);
	free_split(cmd);
	exit(126);
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
	{
		exec(args, env);
		exit(127);
	}
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
	/*
	 * TESTING ARGS
	*/
	for (int z = 0; args[z]; z++) {
		printf("ARG %d: %s\n", z, args[z]);
	}
	// -------------------	
	int	i;

	i = 0;
	if (!args || !args[i] || (parse_redirections(args) == -1))
		return ;
	while (args[i])
	{
		if (ft_strcmp(args[i], "echo") == 0)
		{
			builtin_echo(args);
			return ;
		}
		else if (ft_strcmp(args[i], "cd") == 0)
		{
			builtin_cd(args);
			return ;
		}
		else if (ft_strcmp(args[i], "pwd") == 0)
		{
			builtin_pwd();
			return ;
		}
		/*
		else if (ft_strcmp(args[i], "export") == 0)
		{
			builtin_export();
			return ;
		}
		else if (ft_strcmp(args[i], "unset") == 0)
		{
			builtin_unset();
			return ;
		}
		*/
		else if (ft_strcmp(args[i], "env") == 0)
		{
			builtin_env(env);
			return ;
		}
		else if (ft_strcmp(args[i], "exit") == 0)
		{
			builtin_exit(args);
			return ;
		}
		i++;
	}
	shell_exec(args, env);
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
		cwd = get_cwd();
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
		free(line);
		printf("%s > ", cwd);
		free(cwd);
	}
	return (EXIT_SUCCESS);
}
