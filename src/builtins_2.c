#include "../inc/minishell.h"

// UNSET, ENV, EXIT
/*
void	builtin_unset()
{

}
*/

void	builtin_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

void	builtin_exit(char **args, int *arg_pos)
{
	int exit_code;

	exit_code = 0;
	if (args[*arg_pos])
		exit_code = ft_atoi(args[*arg_pos]);
	exit(exit_code);
}
