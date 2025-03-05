/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:28:33 by jorexpos          #+#    #+#             */
/*   Updated: 2024/09/26 12:03:59 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include <stddef.h>
#include "libft.h"

void	ft_lstclear(t_list **list, void (*del)(void *))
{
	t_list	*current_node;
	t_list	*next_node;

	current_node = *list;
	if (!(current_node))
		return ;
	while (current_node)
	{
		next_node = current_node->next;
		del(current_node->content);
		free(current_node);
		current_node = next_node;
	}
	*list = NULL;
}
