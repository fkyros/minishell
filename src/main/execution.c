#include "../../inc/minishell.h"

void exec_command(char **argv, char **envp)
{
    char    *path;
    struct   stat st;

    if (!argv[0])
        exit(EXIT_SUCCESS);
    path = search_command(argv[0], envp);
    if (!path)
    {
        fprintf(stderr, "%s: command not found\n", argv[0]);
        exit(127);
    }
    if (stat(path, &st) == 0)
    {
        if (S_ISDIR(st.st_mode))
        {
            fprintf(stderr, "%s: Is a directory\n", path);
            free(path);
            exit(126);
        }
        if (access(path, X_OK) != 0)
        {
            fprintf(stderr, "%s: Permission denied\n", path);
            free(path);
            exit(126);
        }
    }
    execve(path, argv, envp);
    perror(argv[0]);
    free(path);
    exit(127);
}
