#include "../inc/minishell.h"

int	open_file(char *file, int in_or_out)
{
	int		fd;
	mode_t	mode;

	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	fd = -1;
	if (in_or_out == 0)
		fd = open(file, O_RDONLY);
	if (in_or_out == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, mode);
	if (fd == -1)
	{
		perror("Error opening the file");
		exit(1);
	}
	return (fd);
}

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

void	print_prompt(char *cwd)
{
	write(STDOUT_FILENO, cwd, strlen(cwd));
	write(STDOUT_FILENO, " > ", 3);
}

/*
char	*ft_strtok(char *str, const char *delim)
{
	static char	*next_token = NULL;
	char	*start;
	char	*end;
	if (str != NULL)
		next_token = str;
	if (next_token == NULL)
		return (NULL);
	start = next_token;
	while (*start && ft_strchr(delim, *start))
		start++;
	if (*start == '\0')
	{
		next_token = NULL;
		return (NULL);
	}
	end = start;
	while (*end && !ft_strchr(delim, *end))
		end++;
	if (*end != '\0')
	{
		*end = '\0';
		next_token = end + 1;
	}
	else
		next_token = NULL;
	return (start);
}
*/
