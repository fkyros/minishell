#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <termcap.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

// COLOURS!
# define RST    "\001\x1b[0m\002" // RESET COLOUR
# define RED    "\x1b[31m"
# define GREEN  "\001\x1b[32m\002"
# define BLUE   "\x1b[34m"
# define PINK   "\001\x1b[35m\002"  
# define BLINK   "\x1b[5m"
# define BOLD    "\001\x1b[1m\002"
# define UNDERLINE "\x1b[4m"

// QUOTE PARSER!
# define STATE_NONE 0
# define STATE_SINGLE 1
# define STATE_DOUBLE 2


# define MAX_CWD 2048

# define GENERIC_ERROR 1
# define BUILTIN_MISUSE 2
# define COMMAND_NOT_FOUND 127
# define COMMAND_NOT_EXECUTABLE 126
# define MAX_ERROR 255

enum e_redirect_type 
{in, out, append, heredoc};

typedef struct s_redirect {
   enum e_redirect_type type;
   char *filename;
   char *heredoc_eof;
} t_redirect;

typedef struct s_command {
	char    **argv;
	t_redirect *redirs;
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
	char		**our_env;
	int			last_status;
	int			saved_stdin;
	int			saved_stdout;
	t_parse_result	*parse_result;
}	t_mini;

typedef struct s_dollar
{
	const char	*input;
	int			*index;
	t_mini		*mini;
	char		*buf;
	size_t		*pos;
}	t_dollar;

typedef struct s_token_detect
{
    const char  *input;
    int         *index;
}	t_token_detect;

typedef struct s_word_process
{
    const char  *input;
    int         *index;
    t_mini      *mini;
    char        *buffer;
    size_t      *buf_pos;
    int         *quote_st;
    int         *flag_quoted;
}	t_word_process;

// UTILS

void 			exec_command(char **argv, char **envp);
char			*ft_getenv(char *name, char **env);
int				print_path_error(char *path, t_parse_result *result, int i);
int				is_operator(const char *token);
char			*expand_variable(const char *line, int *i, t_mini *mini);

// QUOTE PARSING
int				quote_state_after(int state, char quote);
void			append_to_buf(char *buf, size_t *pos, const char *src, size_t len);
void			handle_dollar(t_dollar *context);
// ENV
char			*expand(char *var, t_mini *mini);
char			**init_env(char **old_env);
int				is_var_already_in_env(char *name, char *var_from_env);
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

char    		*get_cwd(t_mini *mini);
void			print_banner(void);
void			signal_handler(int sig);
void			setup_signals(void);
int				run_prompt_loop(t_mini *mini);
int				get_readline_flag(void);
void			set_readline_flag(int value);


// BUILTINS

int				get_num_args(char **args);
int				apply_redirections(t_command *cmd);
int				is_builtin(char *cmd);
int				execute_builtin(t_command *cmd, t_mini *mini);
void			builtin_echo(char **args, t_mini *mini);
void    		builtin_cd(char **args, t_mini *mini);
void			builtin_pwd(t_mini *mini);
void    		builtin_env(t_mini *mini);
void			builtin_export(char **args, t_mini *mini);
void			builtin_unset(char **args, t_mini *mini);
void			builtin_exit(char **args, t_mini *mini);

// HEREDOC
int				check_heredocs(t_parse_result *result, t_mini *mini);
void    		close_heredocs(t_parse_result *result);
char 			*expand_line(const char *line, t_mini *mini);

// PARSING

char			*detect_empty_literal(t_token_detect *tokens);
char			*detect_double_redir(t_token_detect *tokens);
char			*detect_single_op(t_token_detect *tokens);
int				process_quote_or_dollar(t_word_process *words);
void			process_normal_char(t_word_process *words);
char			*consume_special(const char *s, int *i);
char			*consume_word(const char *s, int *i, t_mini *mini);
int				is_after_heredoc(const char *s, int idx);
int				handle_operator_tokens(const char *str, int *index);
char			*get_next_token(const char *s, int *i, t_mini *mini);

// REDIRECTION PARSING
// AUX FUNCTIONS
int				check_unclosed_quotes(const char *str);
int				count_tokens(const char *str);
int				is_whitespace(char c);
int				is_quote(char c);
void			skip_quoted_section(const char *str, int *index, char quote_char);
void			skip_unquoted_section(const char *str, int *index);
char    		*ft_strjoin_char(const char *s1, char c);
char			*extract_raw_heredoc_delim(const char *input, const char *eof_clean);
int				is_pipe(const char *token);
int				is_redirection(const char *token);
enum e_redirect_type	get_redirection_type(const char *token);
int				init_parse_result(const char *input, t_parse_result *result, t_mini *mini);
void			init_command(t_command *cmd, char **argv_start, int is_first);
int				fill_command(char **args, int *i, t_command *cmd, const char *input);
void			compact_argv(char **argv, int length);
int				process_fill_and_empty(t_parse_result *res, int *i, const char *input);
int				check_extra_pipes(char **args, int i);
int				check_invalid_pipes(t_parse_result *result, int *i);

// MAIN FUNCTIONS
void			add_redirect(t_command *cmd, enum e_redirect_type type, char *filename, char *heredoc_eof);
char 			**parse_command(const char *cmd, int *token_count, t_mini *mini);
t_parse_result	parse_commands(const char *input, t_mini *mini);

// PIPING
void    		open_close_pipe(t_parse_result *result, int *i, int (*pipe_fd)[2]);
pid_t			*alloc_pids(int count);
void 			execute_pipeline(t_parse_result *result, t_mini *mini);

// --> PIPING AUX & PROCESSES
void			open_close_pipe(t_parse_result *result, int *i, int (*pipe_fd)[2]);
void			spawn_commands(t_parse_result *res, t_mini *mini);
void			child_branch(int i, t_parse_result *res, t_mini *mini);
void			print_all_child_errors(t_parse_result *res, int *exit_codes);
void			update_exit_code(int *exit_codes, int idx, int status);
int				find_pid_index(pid_t *pids, pid_t pid, int count);
int				*alloc_and_zero_exit_codes(int count);


// SAFE FUNCTIONS
int				safe_chdir(char *dir);
int				safe_dup2(int fd1, int fd2);
int				safe_fork();
void			safe_pipe(int fd[2]);

#endif
