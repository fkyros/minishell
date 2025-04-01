#include "../inc/minishell.h"

void	signal_handling(int sig)
{
	(void)sig;
	char *cwd = get_cwd();

	printf(BOLD GREEN"%s > "RST, cwd);
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	free(cwd);
}

int	main(int argc, char **argv, char **env)
{
	//REPL
	char	*cwd;
	char	*line;
	t_parse_result  parse_result;

	(void)argc;
	(void)argv;

	print_banner();
	signal(SIGINT, signal_handling);
	signal(SIGQUIT, SIG_IGN);
	cwd = get_cwd();
	printf(BOLD GREEN"%s > "RST, cwd);
	free(cwd);
	while (1)
	{
		line = readline(BOLD PINK"funny shell > "RST);
		if (!line)
		{
			printf(UNDERLINE BOLD PINK"Minishell: exiting!\n"RST);
			break ;
		}
		if (*line)
		{
			add_history(line);
			parse_result = parse_commands(line);
			if (parse_result.cmd_count > 0)
                execute_pipeline(&parse_result, env);
			free_commands(&parse_result);
			free(line);
		}
		cwd = get_cwd();
		printf(BOLD GREEN"%s > "RST, cwd);
		free(cwd);	
	}
	return (EXIT_SUCCESS);
}
