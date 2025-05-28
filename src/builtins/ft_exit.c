/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:35:49 by gade-oli          #+#    #+#             */
/*   Updated: 2025/05/28 13:34:54 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	is_valid_num_input(char *arg)
{
	if (arg[0] == '-')
	{
		if (ft_strlen(arg) < ft_strlen("-9223372036854775808"))
			return (1);
		else if (ft_strlen(arg) > ft_strlen("-9223372036854775808"))
			return (0);
		else if (ft_strcmp(arg, "-9223372036854775808") <= 0)
			return (1);
		else if (ft_strcmp(arg, "-9223372036854775808") > 0)
			return (0);
	}
	if (ft_strlen(arg) < ft_strlen("9223372036854775807"))
		return (1);
	else if (ft_strlen(arg) > ft_strlen("9223372036854775807"))
		return (0);
	else if (ft_strcmp(arg, "9223372036854775807") <= 0)
		return (1);
	else if (ft_strcmp(arg, "9223372036854775807") > 0)
		return (0);
	return (0);
}

static int	ft_isnotnum(char *str)
{
	int	i;

	i = 0;

	if (!is_valid_num_input(str))
		return (1);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}

static void	clean_exit(t_mini *mini, int status)
{
	rl_clear_history();
	free_array(mini->our_env);
	free_commands(mini->parse_result);
	free(mini);
	exit(status);
}

void	builtin_exit(char **args, t_mini *mini)
{
	int	status;

	status = 0;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (args[1] && ft_isnotnum(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		status = BUILTIN_MISUSE;
	}
	else if (get_num_args(args) > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		mini->last_status = GENERIC_ERROR;
		return ;
	}
	else if (args[1])
		status = ft_atoi(args[1]) % 256;
	clean_exit(mini, status);
}
