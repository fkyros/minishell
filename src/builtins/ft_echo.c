/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:35:20 by gade-oli          #+#    #+#             */
/*   Updated: 2025/05/25 23:35:22 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	is_valid_n_flag(char *arg)
{
	int	i;

	i = 1;
	if (arg[0] != '-' || arg[1] != 'n')
		return (0);
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}

static int	echo_args(char **args, int *i)
{
	int	newline;

	newline = 1;
	while (args[*i] && is_valid_n_flag(args[*i]))
	{
		newline = 0;
		(*i)++;
	}
	return (newline);
}

void	builtin_echo(char **args, t_mini *mini)
{
	int	i;
	int	newline;

	i = 1;
	newline = echo_args(args, &i);
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[(i++) + 1])
			write(1, " ", 1);
	}
	if (newline)
		write(1, "\n", 1);
	mini->last_status = 0;
}
