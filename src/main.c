#include "minishell.h"

static void	exec(char *cmd, char **env)
{
	char	**s_cmd;
	char	*path;

	s_cmd = parse_command(cmd);
	if (!s_cmd || !s_cmd[0])
	{
		ft_putendl_fd("Minishell: invalid command!", 2);
		exit(127);
	}
	path = search_command(s_cmd[0], env);
	if (!path)
	{
		ft_putstr_fd("Minishell: command not found!", 2);
		ft_putendl_fd(s_cmd[0], 2);
		free_split(s_cmd);
		exit(127);
	}
	execve(path, s_cmd, env);
	perror("Minishell: error");
	free(path);
	free_split(s_cmd);
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
		exec(args[0], env);
		exit(127);
	}
	else
		waitpid(pid, &status, 0);
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
	char	*line;
	char	**args;

	(void)argc;
	(void)argv;

	while ((line = readline("funny shell > ")))
	{
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
	}
	return (EXIT_SUCCESS);
}
