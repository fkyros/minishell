/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:42:36 by jorexpos          #+#    #+#             */
/*   Updated: 2025/06/01 17:05:53 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	g_readline_flag = 0;

int	get_readline_flag(void)
{
	return (g_readline_flag);
}

void	set_readline_flag(int value)
{
	g_readline_flag = value;
}

t_mini	*init_shell(char **env)
{
	t_mini	*mini;

	setup_signals();
	print_banner();
	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (NULL);
	mini->our_env = init_env(env);
	mini->last_status = 0;
	return (mini);
}

int	main(int argc, char **argv, char **env)
{
	t_mini	*mini;
	int		saved_stdin;
	int		saved_stdout;

	(void)argc;
	(void)argv;
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
