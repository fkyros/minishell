#include "../inc/minishell.h"

static int	has_no_whitespaces(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
			return (1);
		i++;
	}
	return (0);
}

static char	*build_prompt(t_mini *mini)
{
	char	*cwd;
	char	*base;
	char	*prompt;
	char	*tmp;

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

static void	exec_line(t_mini *mini, char *line)
{
	t_parse_result	parse_result;

	if (has_no_whitespaces(line))
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

int	run_prompt_loop(t_mini *mini)
{
	char			*line;
	char			*prompt;

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
		if (line[0] == '\f' && line[1] == '\0')
		{
			write(STDOUT_FILENO, "\033[H\033[2J", 7);
			free(line);
			continue ;
		}
		exec_line(mini, line);
	}
}
