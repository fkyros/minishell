/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:39:00 by gade-oli          #+#    #+#             */
/*   Updated: 2025/04/15 19:39:02 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// UNSET, EXPORT, ENV, EXIT
/*
void	builtin_unset()
{

}
*/

void	builtin_export(char **args, char **our_env)
{
	(void)our_env;
	if (!args[1])
	{
		printf("no behaviour defined\n");
		return ;
	}
}

void	builtin_env(char **our_env)
{
	int i;

	i = 0;
	while (our_env[i])
	{
		printf("%s\n", our_env[i]);
		i++;
	}
}

/*
void	builtin_exit()
{
}
*/
