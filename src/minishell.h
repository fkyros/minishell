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
# include "../libft/libft.h"

// COLOURS!
# define RST    "\x1b[0m" // RESET COLOUR
# define RED    "\x1b[31m"
# define GREEN  "\x1b[32m"
# define BLUE   "\x1b[34m"
# define PINK   "\x1b[35m"  
# define BLINK   "\x1b[5m"
# define BOLD    "\x1b[1m"
# define UNDERLINE "\x1b[4m"



typedef struct s_command {
	char    **argv;
	char    *redirect_in;
	char    *heredoc;
	char    *redirect_out;
	char    *redirect_append;
	int     pipe_out;
	int     is_first;
	int     is_last;
}   t_command;


typedef struct s_parse_result {
    t_command *commands;
    char **original_tokens;
    char **args;  
    int token_count;
    int cmd_count;
} t_parse_result;

// UTILS

int	open_file(char *file, int in_or_out);
void	free_split(char **split);
char	*ft_getenv(char *name, char **env);



// PATHING

int	is_valid_path(char *cmd);
char	*build_full_path(char *path, char *cmd);
char	*search_in_path(char **paths, char *cmd);
char	*search_command(char *cmd, char **env);

// SHELL UTILITIES

char    *get_cwd(void);
void	print_banner(void);

// BUILTINS

void	builtin_echo(char **args, int *arg_pos);
void    builtin_cd(char **args, int *arg_pos);
void    builtin_pwd(void);
void    builtin_env(char **env);
void	builtin_exit(char **args, int *arg_pos);

// REDIRECTION PARSING
// 		-- AUX FUNCTIONS
int	count_tokens(const char *str);
int	is_whitespace(char c);
int	is_quote(char c);
void	skip_quoted_section(const char *str, int *index, char quote_char);
void	skip_unquoted_section(const char *str, int *index);
// 		-- MAIN FUNCTIONS
int     parse_redirections(char **args);
int     handle_pipe(char **args, int i);
char **parse_command(const char *cmd, int *token_count);
t_parse_result	parse_commands(const char *input);
void free_commands(t_parse_result *result);

// PIPING

void setup_input(t_command *cmd, int prev_pipe_fd);
void setup_output(t_command *cmd, int pipe_fd[2]);
void setup_pipes_and_redirection(t_command *cmd, int prev_pipe_fd, int pipe_fd[2]);
void execute_pipeline(t_parse_result *result, char **env);

#endif
