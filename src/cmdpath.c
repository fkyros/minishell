#include "../inc/minishell.h"

int	is_valid_path(char *cmd)
{
	return (access(cmd, X_OK) == 0);
}

char	*build_full_path(char *path, char *cmd)
{
	char	*temp_buffer;
	char	*full_path;

	temp_buffer = ft_strjoin(path, "/");
	if (!temp_buffer)
		return (NULL);
	full_path = ft_strjoin(temp_buffer, cmd);
	free(temp_buffer);
	return (full_path);
}

char	*search_in_path(char **paths, char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = build_full_path(paths[i], cmd);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}

char	*search_command(char *cmd, char **env)
{
	char	**paths;

	if (!env || !cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (is_valid_path(cmd))
			return (cmd);
		return ("/");
	}
	paths = ft_split(ft_getenv("PATH", env), ':');
	if (!paths)
		return (NULL);
	return (search_in_path(paths, cmd));
}
