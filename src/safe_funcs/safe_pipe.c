/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 14:17:46 by gade-oli          #+#    #+#             */
/*   Updated: 2025/06/01 14:18:29 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	safe_pipe(int fd[2])
{
	int	res;

	res = pipe(fd);
	if (res < 0)
	{
		perror("minishell: pipe");
		exit(EXIT_FAILURE);
	}
}
