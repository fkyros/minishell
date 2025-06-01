/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_aux.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:44:01 by jorexpos          #+#    #+#             */
/*   Updated: 2025/06/01 16:44:01 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	check_unclosed_quotes(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
			if (!str[i])
				return (0);
		}
		else if (str[i] == '\"')
		{
			i++;
			while (str[i] && str[i] != '\"')
				i++;
			if (!str[i])
				return (0);
		}
		if (str[i])
			i++;
	}
	return (1);
}

void	skip_quoted_section(const char *str, int *index, char quote_char)
{
	(*index)++;
	while (str[*index] && str[*index] != quote_char)
		(*index)++;
	if (str[*index])
		(*index)++;
}

void	skip_unquoted_section(const char *str, int *index)
{
	while (str[*index] && !is_whitespace(str[*index]) && !is_quote(str[*index]))
		(*index)++;
}
