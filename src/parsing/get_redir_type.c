/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redir_type.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:43:50 by jorexpos          #+#    #+#             */
/*   Updated: 2025/06/01 16:43:50 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_pipe(const char *token)
{
	if (!token)
		return (0);
	if (token[0] == '\x02')
		return (0);
	return (ft_strcmp(token, "|") == 0);
}

int	is_redirection(const char *token)
{
	if (!token)
		return (0);
	if (token[0] == '\x02')
		return (0);
	return (ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, ">>") == 0
		|| ft_strcmp(token, "<") == 0
		|| ft_strcmp(token, "<<") == 0);
}

int	is_operator(const char *token)
{
	if (!token)
		return (0);
	return (ft_strcmp(token, "|") == 0
		|| ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, ">>") == 0
		|| ft_strcmp(token, "<") == 0
		|| ft_strcmp(token, "<<") == 0);
}

enum e_redirect_type	get_redirection_type(const char *token)
{
	if (ft_strcmp(token, ">") == 0)
		return (out);
	if (ft_strcmp(token, ">>") == 0)
		return (append);
	if (ft_strcmp(token, "<") == 0)
		return (in);
	return (heredoc);
}

int	handle_operator_tokens(const char *str, int *index)
{
	int	count;

	count = 0;
	if ((str[*index] == '<' && str[*index + 1] == '<')
		|| (str[*index] == '>' && str[*index + 1] == '>'))
	{
		count++;
		*index += 2;
		return (count);
	}
	if (str[*index] == '<' || str[*index] == '>' || str[*index] == '|')
	{
		count++;
		(*index)++;
		return (count);
	}
	return (0);
}
