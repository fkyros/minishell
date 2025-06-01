/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raw_delim.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:44:45 by jorexpos          #+#    #+#             */
/*   Updated: 2025/06/01 16:44:46 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static char	*extract_if_match(const char *start, const char *end,
	const char *q, const char *eof_clean)
{
	size_t	clean_len;
	size_t	offset;
	char	*clean;
	char	*raw;

	clean_len = end - q;
	clean = ft_substr(q, 0, clean_len);
	if (!clean)
		return (NULL);
	if (ft_strcmp(clean, eof_clean) == 0)
	{
		offset = 0;
		if (is_quote(*start))
			offset = 1;
		raw = ft_substr(start, 0, (end - start) + offset);
		free(clean);
		return (raw);
	}
	free(clean);
	return (NULL);
}

static char	*process_heredoc_candidate(const char *p,
	const char *eof_clean, const char **next_p)
{
	const char	*ptr;
	char		quote;
	const char	*start;
	const char	*end;
	char		*match;

	ptr = p + 2;
	quote = skip_spaces_and_get_quote(&ptr);
	if (!get_bounds(ptr, quote, &start, &end))
		return (NULL);
	match = extract_if_match(start, end, ptr, eof_clean);
	if (match)
		return (match);
	if (quote)
		*next_p = end + 1;
	else
		*next_p = end;
	return (NULL);
}

char	*extract_raw_heredoc_delim(const char *input, const char *eof_clean)
{
	const char	*p;
	char		*match;
	const char	*next_p;

	p = find_next_heredoc(input);
	while (p != NULL)
	{
		match = process_heredoc_candidate(p, eof_clean, &next_p);
		if (match)
			return (match);
		p = find_next_heredoc(next_p);
	}
	return (NULL);
}
