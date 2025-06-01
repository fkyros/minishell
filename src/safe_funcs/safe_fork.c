/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 14:17:39 by gade-oli          #+#    #+#             */
/*   Updated: 2025/06/01 14:19:00 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	safe_fork(void)
{
	int	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}
