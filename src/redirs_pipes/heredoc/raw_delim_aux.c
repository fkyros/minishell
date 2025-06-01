/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raw_delim_aux.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:44:49 by jorexpos          #+#    #+#             */
/*   Updated: 2025/06/01 16:44:51 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

const char	*find_next_heredoc(const char *p)
{
	return (ft_strstr(p, "<<"));
}

char	skip_spaces_and_get_quote(const char **q)
{
	char	quote;

	while (**q && is_whitespace(**q))
		(*q)++;
	quote = 0;
	if (is_quote(**q))
	{
		quote = **q;
		(*q)++;
	}
	return (quote);
}

int	get_bounds(const char *q, char quote,
	const char **start, const char **end)
{
	*start = q;
	if (quote)
	{
		*start = q - 1;
		*end = ft_strchr(q, quote);
		if (!*end)
			return (0);
	}
	else
	{
		*end = q;
		while (**end
			&& !is_whitespace(**end)
			&& **end != '<'
			&& **end != '>'
			&& **end != '|')
			(*end)++;
	}
	return (1);
}
