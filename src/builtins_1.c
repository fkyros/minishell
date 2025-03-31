#include "../inc/minishell.h"

// ECHO, CD, PWD, EXPORT

void	builtin_echo(char **args, int *arg_pos)
{
	int	i;
	int	newline;

	i = *arg_pos;
	newline = 1;
	if (args[i + 1] && (ft_strcmp(args[i + 1], "-n") == 0))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i + 1], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", 1);
}

void	builtin_cd(char **args, int *arg_pos)
{
	char	*home;

	if (!args[*arg_pos + 1])
	{
		home = getenv("HOME");
		if (home)
		{
			if (chdir(home) != 0)
				perror("An error has ocurred while trying to use 'cd'");
		}
		else
			ft_putstr_fd("Minishell: cd: HOME not set", 2);
	}
	else
	{
		if (chdir(args[*arg_pos + 1]) != 0)
			perror("An error has ocurred while trying to use 'cd'");
	}
}

void	builtin_pwd(void)
{
	char	*cwd;

	cwd = get_cwd();
	printf("%s\n", cwd);
	free(cwd);
}

/*
void	builtin_export()
{

}
*/
