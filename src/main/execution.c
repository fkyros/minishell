#include "../../inc/minishell.h"

static void exit_if_empty_command(char **argv)
{
    if (argv[0] && argv[0][0] == '\0')
        return ;
    if (!argv[0])
        return ;
}

static void exec_if_path(char **argv, char **envp)
{
    struct stat st;

    if (stat(argv[0], &st) < 0)
        return ;
    if (S_ISDIR(st.st_mode))
    {
        errno = EISDIR;
        return ;
    }
    if (access(argv[0], X_OK) != 0)
    {
        errno = EACCES;
        return  ;
    }
    execve(argv[0], argv, envp);
    return ;
}

static void exec_from_env(char **argv, char **envp)
{
    char       *path;
    struct stat st;

    path = search_command(argv[0], envp);
    if (!path)
        return ;

    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
    {
        free(path);
        return ;
    }
    if (access(path, X_OK) != 0)
    {
        free(path);
        return ;
    }
    execve(path, argv, envp);
    free(path);
    return ;
}

void exec_command(char **argv, char **envp)
{
    exit_if_empty_command(argv);
    if (!argv[0])
        return;

    if (ft_strchr(argv[0], '/') != NULL)
        exec_if_path(argv, envp);
    else
        exec_from_env(argv, envp);
}


