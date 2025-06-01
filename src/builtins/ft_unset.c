/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:01:39 by gade-oli          #+#    #+#             */
/*   Updated: 2025/06/01 17:01:44 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	unset_one_var(char *arg, t_mini *mini)
{
	char	**new_env;
	int		j;
	int		flag;

	j = 0;
	flag = 0;
	new_env = NULL;
	while (!flag && mini->our_env[j])
	{
		if (is_var_already_in_env(arg, mini->our_env[j]))
			flag = 1;
		j++;
	}
	if (flag)
		new_env = delete_var_from_env(arg, mini->our_env);
	if (new_env)
	{
		free_array(mini->our_env);
		mini->our_env = new_env;
	}
}

void	builtin_unset(char **args, t_mini *mini)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!invalid_var_name(args[i]) && ft_strcmp(args[i], "_"))
			unset_one_var(args[i], mini);
		i++;
	}
	mini->last_status = 0;
}
