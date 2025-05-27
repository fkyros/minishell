/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_dup2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 01:03:20 by gade-oli          #+#    #+#             */
/*   Updated: 2025/05/28 01:05:41 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	safe_dup2(int fd1, int fd2)
{
	int	res;

	res = dup2(fd1, fd2);
	if (res == -1)
		perror("minishell: dup2");
	return (res);
}
