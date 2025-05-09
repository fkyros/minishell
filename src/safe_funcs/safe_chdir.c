/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_chdir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 19:32:34 by gade-oli          #+#    #+#             */
/*   Updated: 2025/05/09 19:37:09 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//INTENDED USE: FOR CD BUILTIN
int	safe_chdir(char *dir)
{
	int	status;

	status = chdir(dir);
	if (status != 0)
		perror(BOLD RED"minishell error: cd"RST);
	return (status);
}
