/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:35:32 by gade-oli          #+#    #+#             */
/*   Updated: 2025/05/25 23:36:41 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_env(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini->our_env[i])
	{
		printf("%s\n", mini->our_env[i]);
		i++;
	}
	mini->last_status = 0;
}
