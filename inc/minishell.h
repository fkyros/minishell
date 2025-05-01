#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <linux/limits.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <termcap.h>
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

# define MAX_CWD 2048

# define GENERIC_ERROR 1
# define COMMAND_NOT_FOUND 127
# define COMMAND_NOT_EXECUTABLE 126

enum e_redirect_type 
{in, out, append, heredoc};

typedef struct s_redirect {
   enum e_redirect_type type;
   char *filename;
   char *heredoc_eof;
} t_redirect;

typedef struct s_command {
	char    **argv;
	t_redirect *redirs; // ALREADY ORDERED LINKED LIST
	int		redir_count;
	char    *heredoc;
	int     pipe_out;
	int     is_first;
	int     is_last;
	int     heredoc_fd;
}   t_command;

typedef struct s_parse_result {
    t_command *commands;
    char **original_tokens;
    char **args;  
    int token_count;
    int cmd_count;
}	t_parse_result;

typedef	struct s_mini
{
	char	**our_env;
	int		last_status;
}	t_mini;


// UTILS

void			free_split(char **split);
char			*ft_getenv(char *name, char **env);
int				print_path_error(char *path, t_parse_result *result, int i);
int				is_operator(const char *token);

// ENV
char			*expand(char *var, t_mini *mini);
char			**init_env(char **old_env);
int			is_var_already_in_env(char *name, char *var_from_env);
char			**add_var_to_env(char **our_env, char *name, char *value);
char			**delete_var_from_env(char *name, char **our_env);

// PATHING

int				is_valid_path(char *cmd);
char			*build_full_path(char *path, char *cmd);
char			*search_in_path(char **paths, char *cmd);
char			*search_command(char *cmd, char **env);

// MEMORY HANDLING

void    		*ft_realloc(void *ptr, size_t old_size, size_t new_size);
void			free_array(char **arr);
void 			free_commands(t_parse_result *result);

// SHELL UTILITIES

char    		*get_cwd(void);
void			print_banner(void);

// BUILTINS

int				apply_redirections(t_command *cmd);
int				is_builtin(char *cmd);
void  			execute_builtin(t_command *cmd, int apply_redirects, t_mini *mini);
void			builtin_echo(char **args, t_mini *mini);
void    		builtin_cd(char **args, t_mini *mini);
void			builtin_pwd(t_mini *mini);
void    		builtin_env(t_mini *mini);
void			builtin_exit(char **args);
void			builtin_export(char **args, t_mini *mini);
void			builtin_unset(char **args, t_mini *mini);

// HEREDOC
void			check_heredocs(t_parse_result *result);
void    		close_heredocs(t_parse_result *result);

// REDIRECTION PARSING
// AUX FUNCTIONS
int				check_unclosed_quotes(const char *str);
int				count_tokens(const char *str);
int				is_whitespace(char c);
int				is_quote(char c);
void			skip_quoted_section(const char *str, int *index, char quote_char);
void			skip_unquoted_section(const char *str, int *index);
// MAIN FUNCTIONS
void			add_redirect(t_command *cmd, enum e_redirect_type type, char *filename, char *heredoc_eof);
char 			**parse_command(const char *cmd, int *token_count, t_mini *mini);
t_parse_result	parse_commands(const char *input, t_mini *mini);

// PIPING
void 			setup_input(t_command *cmd, int prev_pipe_fd);
void 			setup_output(t_command *cmd, int (*pipe_fd)[2]);
void 			setup_pipes_and_redirection(t_command *cmd, int prev_pipe_fd, int (*pipe_fd)[2]);
void 			execute_pipeline(t_parse_result *result, t_mini *mini);

// --> PIPING AUX
void			open_close_pipe(t_parse_result *result, int *i, int (*pipe_fd)[2]);
void 			child_process(t_parse_result *result, int *i, int (*pipe_fd)[2], int *prev_pipe_fd, t_mini *mini);
void    		parent_process(t_parse_result *result, int *i, int (*pipe_fd)[2], int *prev_pipe_fd);
void    		process_handling(int *pid, t_parse_result *result, int *i, int (*pipe_fd)[2], int *prev_pipe_fd, t_mini *mini);
void 			wait_processes(t_parse_result *result, t_mini *mini);

#endif
