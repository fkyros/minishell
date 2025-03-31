#include "../inc/minishell.h"


void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	*ft_getenv(char *name, char **env)
{
	int		i;
	int		j;
	char	*sub;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j);
		if (ft_strcmp(sub, name) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
		i++;
	}
	return (NULL);
}

char    *get_cwd(void)
{
        char    *cwd = malloc(2048);
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

