#include "../inc/minishell.h"

// UNSET, EXPORT, ENV, EXIT
/*
void	builtin_unset()
{

}
*/

/*
void	builtin_export(char **args, char **our_env)
{
	
}
*/

void	builtin_env(char **our_env)
{
	int i;

	i = 0;
	while (our_env[i])
	{
		printf("%s\n", our_env[i]);
		i++;
	}
}

/*
void	builtin_exit()
{
}
*/
