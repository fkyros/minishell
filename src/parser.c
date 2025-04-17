#include "../inc/minishell.h"

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

static char	*get_next_token(const char *str, int *index, char **our_env)
{
	char	*res;

	while (str[*index] && is_whitespace(str[*index]))
		(*index)++;
	res = extract_token(str, index);
	if (res && ft_strncmp(res, "$", 1) == 0)
		return (expand(res, our_env));
	return (res);
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

char **parse_command(const char *cmd, int *token_count, char **our_env) 
{
    int index = 0;
    int count = count_tokens(cmd);
    char **tokens = malloc((count + 1) * sizeof(char *));
	if (!cmd && !tokens)
	{
		free(tokens);
		return (NULL);
	}
    *token_count = count;
    index = 0;
    int i = 0;
    while (i < count) {
        tokens[i] = get_next_token(cmd, &index, our_env);
        i++;
    }
    tokens[i] = NULL;
    return (tokens);
}
