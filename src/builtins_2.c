/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:39:00 by gade-oli          #+#    #+#             */
/*   Updated: 2025/04/20 20:56:38 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// UNSET, EXPORT, ENV, EXIT
/*
void	builtin_unset()
{

}
*/

void	builtin_export(char **args, t_mini *mini)
{
	int		i;
	char	**var;
	char	**new_env;
	int		status;

	status = 0;
	i = 1;
	if (!args[i])
	{
		ft_putstr_fd("no behaviour defined\n", STDERR_FILENO);
		ft_putstr_fd("see more at https://www.man7.org/linux/man-pages/man1/export.1p.html\n", STDERR_FILENO);
		return ;
	}
	while(args[i])
	{
		if (!ft_isalpha(args[i][0]) || !ft_strchr(args[i], '='))
		{
			ft_putstr_fd("Export with an invalid identifier\n", STDERR_FILENO);
			status = 1;
		}
		else
		{
			var = ft_split(args[i], '=');
			new_env = add_var_to_env(mini->our_env, var[0], var[1]);
			if (!new_env)
				ft_putstr_fd("Error adding var to env\n", STDERR_FILENO);
			free_split(var);
		}
		i++;
	}
	if (new_env)
	{
		free_split(mini->our_env);
		mini->our_env = new_env;
	}
	mini->last_status = status;
}

void	builtin_env(t_mini *mini)
{
	int i;

	i = 0;
	while (mini->our_env[i])
	{
		printf("%s\n", mini->our_env[i]);
		i++;
	}
	mini->last_status = 0;
}

/*
void	builtin_exit()
{
}
*/
