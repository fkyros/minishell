#include "minishell.h"

static char	*extract_token(const char *str, int *index)
{
	int		start;
	int		len;
	char	quote_char;

	if (is_quote(str[*index]))
	{
		quote_char = str[*index];
		start = ++(*index);
		while (str[*index] && str[*index] != quote_char)
			(*index)++;
		len = *index - start;
		(*index)++;
	}
	else
	{
		start = *index;
		skip_unquoted_section(str, index);
		len = *index - start;
	}
	return (ft_substr(str, start, len));
}

static char	*get_next_token(const char *str, int *index)
{
	while (str[*index] && is_whitespace(str[*index]))
		(*index)++;
	return (extract_token(str, index));
}

int	count_tokens(const char *str)
{
	int	index;
	int	count;

	index = 0;
	count = 0;
	while (str[index])
	{
		while (str[index] && is_whitespace(str[index]))
			index++;
		if (str[index])
		{
			count++;
			if (is_quote(str[index]))
				skip_quoted_section(str, &index, str[index]);
			else
				skip_unquoted_section(str, &index);
		}
	}
	return (count);
}

char	**parse_command(const char *cmd)
{
	int		index;
	int		token_count;
	int		i;
	char	**tokens;

	if (!cmd)
		return (NULL);
	token_count = count_tokens(cmd);
	tokens = (char **)malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	index = 0;
	i = 0;
	while (i < token_count)
	{
		tokens[i] = get_next_token(cmd, &index);
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}
