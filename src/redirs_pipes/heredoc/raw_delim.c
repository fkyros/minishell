#include "../../../inc/minishell.h"

static const char	*find_next_heredoc(const char *p)
{
	return (ft_strstr(p, "<<"));
}

static char	skip_spaces_and_get_quote(const char **q)
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

static int	get_bounds(const char *q, char quote,
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

static char	*extract_if_match(const char *start, const char *end,
								 const char *q, const char *eof_clean)
{
	size_t	clean_len;
	char	*clean;
	char	*raw;

	clean_len = end - q;
	clean = ft_substr(q, 0, clean_len);
	if (!clean)
		return (NULL);
	if (ft_strcmp(clean, eof_clean) == 0)
	{
		raw = ft_substr(start, 0, (end - start)
			+ (is_quote(*start) ? 1 : 0));
		free(clean);
		return (raw);
	}
	free(clean);
	return (NULL);
}

char	*extract_raw_heredoc_delim(const char *input, const char *eof_clean)
{
	const char	*p;
	const char	*ptr;
	const char	*start;
	const char	*end;
	char		quote;

	p = input;
	while ((p = find_next_heredoc(p)) != NULL)
	{
		ptr = p + 2;
		quote = skip_spaces_and_get_quote(&ptr);
		if (!get_bounds(ptr, quote, &start, &end))
			break ;
		{
			char *match = extract_if_match(start, end, ptr, eof_clean);
			if (match)
				return (match);
		}
		if (quote)
			p = end + 1;
		else
			p = end;
	}
	return (NULL);
}
