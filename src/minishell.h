#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../Libft/libft.h"

// UTILS

int	open_file(char *file, int in_or_out);
void	free_split(char **split);
char	*ft_getenv(char *name, char **env);

// PARSER

int	count_tokens(const char *str);
char	**parse_command(const char *cmd);
int	is_whitespace(char c);
int	is_quote(char c);
void	skip_quoted_section(const char *str, int *index, char quote_char);
void	skip_unquoted_section(const char *str, int *index);

// PATHING

int	is_valid_path(char *cmd);
char	*build_full_path(char *path, char *cmd);
char	*search_in_path(char **paths, char *cmd);
char	*search_command(char *cmd, char **env);
#endif
