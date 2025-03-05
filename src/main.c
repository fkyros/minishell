#include "minishell.h"

int	g_child_pid = 0;

void	signal_handling(int sig)
{
	(void)sig;
	
	if (g_child_pid > 0)
		kill(g_child_pid, SIGINT);
	else
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

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

static void	exec(char **cmd, char **env)
{
	char	*path;

	if (!cmd || !cmd[0])
	{
		ft_putendl_fd("Minishell: invalid command!", 2);
		exit(127);
	}
	path = search_command(cmd[0], env);
	if (!path)
	{
		ft_putstr_fd("Minishell: command not found!", 2);
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
		perror("Shell: failed");
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
	}
}

void	shell_start(char **args, char **env)
{
	if (!args || !args[0])
		return ;
	//Builtin handling TO DO
	
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
	while ((line = readline("funny shell > ")))
	{
		if (!line)
		{
			printf("exit\n");
			break;
		}
		if (*line)
		{
			add_history(line);
			args = parse_command(line);
			if (args[0] && !ft_strcmp(args[0], "cd"))
				chdir(args[1]);
			else
				shell_start(args, env);
			free_split(args);
		}
		free(line);
		cwd = get_cwd();
		printf("%s > ", cwd);
		free(cwd);
	}
	return (EXIT_SUCCESS);
}
