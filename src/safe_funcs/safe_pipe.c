#include "../../inc/minishell.h"

void safe_pipe(int fd[2])
{
    int res;
    
    res = pipe(fd);
    if (res < 0)
    {
        perror("minishell: pipe");
		exit(EXIT_FAILURE);
    }
}