/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:45:07 by jorexpos          #+#    #+#             */
/*   Updated: 2025/06/01 16:45:07 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*get_cwd(t_mini *mini)
{
	char	*cwd;

	cwd = malloc(MAX_CWD);
	if (!cwd)
	{
		perror(BOLD RED"minishell: Error trying to assign memory \
			for the current directory"RST);
		exit(1);
	}
	if (!getcwd(cwd, MAX_CWD))
	{
		free(cwd);
		cwd = ft_getenv("PWD", mini->our_env);
	}
	return (cwd);
}

void	print_banner(void)
{
	printf(
		BLINK BOLD PINK"       __  "GREEN" _____   _      "PINK"__  \n"
		PINK"  _   / /"GREEN"  / ____| | |    "PINK" \\ \\ \n"
		PINK" (_) | |  "GREEN"| (___   | |__    "PINK"| |\n"
		PINK"     | |   "GREEN"\\___ \\  | '_ \\   "PINK"| |\n"
		PINK"  _  | |   "GREEN"____) | | | | |  "PINK"| |\n"
		PINK" (_) | |  "GREEN"|_____/  |_| |_|  "PINK"| |\n"
		PINK"      \\_\\                  /_/ "RST"\n\n"
		);
}

int	print_path_error(char *path, t_parse_result *result, int i)
{
	if (path && ft_strcmp(path, "/") == 0)
		ft_putstr_fd(RED BOLD"Minishell: No such file or directory: "
			RST, STDERR_FILENO);
	else
		ft_putstr_fd(RED BOLD"Minishell: Command not found: "
			RST, STDERR_FILENO);
	ft_putendl_fd(result->commands[i].argv[0], STDERR_FILENO);
	free_commands(result);
	return (COMMAND_NOT_FOUND);
}
