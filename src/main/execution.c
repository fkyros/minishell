#include "../../inc/minishell.h"

static void	exit_if_empty_command(char **argv)
{
	if (argv[0] && argv[0][0] == '\0')
	{
		ft_putstr_fd("'' : command not found\n", STDERR_FILENO);
		exit(127);
	}
	if (!argv[0])
		exit(EXIT_SUCCESS);
}

static void exec_if_path(char **argv, char **envp)
{
    struct stat st;
    char       *msg;

    if (stat(argv[0], &st) < 0)
    {
        msg = strerror(errno);
        ft_putstr_fd(argv[0], STDERR_FILENO);
        ft_putstr_fd(": ",   STDERR_FILENO);
        ft_putstr_fd(msg,    STDERR_FILENO);
        ft_putstr_fd("\n",    STDERR_FILENO);
        exit(127);
    }
    if (S_ISDIR(st.st_mode))
    {
        ft_putstr_fd(argv[0], STDERR_FILENO);
        ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
        exit(126);
    }
    if (access(argv[0], X_OK) != 0)
    {
        ft_putstr_fd(argv[0], STDERR_FILENO);
        ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
        exit(126);
    }
    execve(argv[0], argv, envp);
    perror(argv[0]);
    exit(127);
}

static void exec_from_env(char **argv, char **envp)
{
    char      *path;
    struct stat st;

    path = search_command(argv[0], envp);
    if (!path)
    {
        ft_putstr_fd(argv[0], STDERR_FILENO);
        ft_putstr_fd(": command not found\n", STDERR_FILENO);
        exit(127);
    }
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
    {
        ft_putstr_fd(argv[0], STDERR_FILENO);
        ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
        free(path);
        exit(126);
    }
    if (access(path, X_OK) != 0)
    {
        ft_putstr_fd(argv[0], STDERR_FILENO);
        ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
        free(path);
        exit(126);
    }
    execve(path, argv, envp);
    perror(argv[0]);
    free(path);
    exit(127);
}

void	exec_command(char **argv, char **envp)
{
	exit_if_empty_command(argv);
	if (ft_strchr(argv[0], '/') != NULL)
		exec_if_path(argv, envp);
	else
		exec_from_env(argv, envp);
}




