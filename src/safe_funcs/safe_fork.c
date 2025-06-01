#include "../../inc/minishell.h"

int safe_fork()
{
    int pid;

    pid = fork();
    if (pid < 0)
    {
        perror("minishell: fork");
		exit(EXIT_FAILURE);
    }
    return (pid);
}