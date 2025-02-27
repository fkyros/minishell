#ifndef MINISHELL_H
# define MINISHELL_H

# include "../megalibft/libft.h"
# include <string.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

//env.c

/**
 * Finds and expands enviroment variables.
 * 
 * @param matrix Is the whole command to check and expand its variables 
 * @return Returns NULL if any error ocurred, else a copy of matrix with all the variables expanded
 */
char	**expand(char **matrix);

#endif
