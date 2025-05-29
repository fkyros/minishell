#include "../inc/minishell.h"

static int g_readline_flag = 0;

int get_readline_flag(void)
{
	return (g_readline_flag);
}

void set_readline_flag(int value)
{
	g_readline_flag = value;
}

void signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		if (get_readline_flag())
		{
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
	}
	else if (sig == SIGWINCH)
	{
		if (get_readline_flag())
			rl_redisplay();
	}
}

int handle_ctrl_l(int count, int key)
{
	(void)count;
	(void)key;

	write(STDOUT_FILENO, "\033[H\033[2J", 7);

	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	return (0);
}

void setup_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGWINCH, signal_handler);
	rl_bind_key('\f', handle_ctrl_l);
}

char *build_prompt(t_mini *mini)
{
	char *cwd;
	char *base;
	char *prompt;
	char *tmp;

	cwd = get_cwd(mini);
	if (cwd)
	{
		tmp = ft_strjoin(BOLD GREEN, cwd);
		free(cwd);
		base = ft_strjoin(tmp, " > " RST);
		free(tmp);
	}
	else
		base = ft_strdup(BOLD GREEN "minishell > " RST);
	prompt = ft_strjoin(base, BOLD PINK "funny shell > " RST);
	free(base);
	return (prompt);
}

t_mini *init_shell(char **env)
{
	t_mini *mini;
	
	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (NULL);
	mini->our_env = init_env(env);
	mini->last_status = 0;
	return (mini);
}

int run_prompt_loop(t_mini *mini)
{
	t_parse_result parse_result;
	char *line;
	char *prompt;

	while (1)
	{
		prompt = build_prompt(mini);
		set_readline_flag(1);
		line = readline(prompt);
		set_readline_flag(0);
		free(prompt);
		if (!line)
		{
			printf(PINK BOLD"\nMinishell: exiting!\n"RST);
			return (0);
		}
		if (*line)
		{
			add_history(line);
			parse_result = parse_commands(line, mini);
			mini->parse_result = &parse_result;
			if (parse_result.cmd_count > 0)
				execute_pipeline(&parse_result, mini);
			free_commands(&parse_result);
		}
		free(line);
	}
}

int main(int argc, char **argv, char **env)
{
	t_mini *mini;
	int		saved_stdin;
	int		saved_stdout;

	(void)argc;
	(void)argv;
	setup_signals();
	print_banner();
	mini = init_shell(env);
	if (!mini)
		return (1);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdin < 0 || saved_stdout < 0)
    {
        perror("minishell: dup");
        exit(EXIT_FAILURE);
    }
    mini->saved_stdin = saved_stdin;
	mini->saved_stdout = saved_stdout;
	run_prompt_loop(mini);
	free_array(mini->our_env);
	close(mini->saved_stdin);
	close(mini->saved_stdout);
	free(mini);
	return (EXIT_SUCCESS);
}
