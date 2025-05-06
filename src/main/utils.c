#include "../inc/minishell.h"

//TODO: still necessary?
// char	*ft_getenv(char *name, char **env)
// {
// 	int		i;
// 	int		j;
// 	char	*sub;

// 	i = 0;
// 	while (env[i])
// 	{
// 		j = 0;
// 		while (env[i][j] && env[i][j] != '=')
// 			j++;
// 		sub = ft_substr(env[i], 0, j);
// 		if (ft_strcmp(sub, name) == 0)
// 		{
// 			free(sub);
// 			return (env[i] + j + 1);
// 		}
// 		free(sub);
// 		i++;
// 	}
// 	return (NULL);
// }

char    *get_cwd(void)
{
        char    *cwd = malloc(MAX_CWD);
        if (!cwd)
        {
            perror(BOLD RED"minishell: Error trying to assign memory for the current directory"RST);
            exit(1);
        }
        if (!getcwd(cwd, MAX_CWD))
        {
            perror(BOLD RED"minishell: Error trying to obtain current directory"RST);
            free(cwd);
            exit(1);
        }
        return (cwd);
}

void	print_banner(void)
{
printf(                                     
BLINK BOLD PINK"       __  "GREEN" _____   _      "PINK"__  \n"
PINK"  _   / /"GREEN"  / ____| | |    "PINK" \\ \\ \n"
PINK" (_) | |  "GREEN"| (___   | |__    "PINK"| |\n"
PINK"     | |   "GREEN"\\___ \\  | '_ \\   "PINK"| |\n"
PINK"  _  | |   "GREEN"____) | | | | |  "PINK"| |\n"
PINK" (_) | |  "GREEN"|_____/  |_| |_|  "PINK"| |\n"
PINK"      \\_\\                  /_/ "RST"\n\n"                                                              
);	
}

int	print_path_error(char *path, t_parse_result *result, int i)
{
	if (path && ft_strcmp(path, "/") == 0)
		ft_putstr_fd(RED BOLD"Minishell: No such file or directory: "
		RST, STDERR_FILENO);
	else
		ft_putstr_fd(RED BOLD"Minishell: Command not found: "
		RST, STDERR_FILENO);
	ft_putendl_fd(result->commands[i].argv[0], STDERR_FILENO);
	free_commands(result);
	return (COMMAND_NOT_FOUND);
}
