#include "../inc/minishell.h"

char	*str_append(char *s1, const char *s2)
{
	char	*new_str;
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	new_str = malloc(len1 + len2 + 1);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, s1, len1 + 1);
	ft_strlcat(new_str, s2, len1 + len2 + 1);
	free(s1);
	return (new_str);
}


static char	*extract_var_name(const char *line, int *i)
{
	int		start;

	start = *i;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	return (ft_substr(line, start, *i - start));
}

static char	*expand_status(int *i, t_mini *mini)
{
	*i += 2;
	return (ft_itoa(mini->last_status));
}

static char	*expand_variable(const char *line, int *i, t_mini *mini)
{
	char	*var;
	char	*dollar_var;
	char	*value;

	(*i)++;
	var = extract_var_name(line, i);
	if (!var)
		return (NULL);
	dollar_var = ft_strjoin("$", var);
	free(var);
	value = expand(dollar_var, mini);
	free(dollar_var);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static char	*get_expanded_value(const char *line, int *i, t_mini *mini)
{
	if (line[*i + 1] == '?')
		return (expand_status(i, mini));
	return (expand_variable(line, i, mini));
}

static char	*append_char(char *dst, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (str_append(dst, tmp));
}

char	*expand_line(const char *line, t_mini *mini)
{
	int		i;
	char	*result;
	char	*expanded;

	i = 0;
	result = ft_calloc(1, 1);
	if (!result)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '$' && (ft_isalnum(line[i + 1])
				|| line[i + 1] == '_' || line[i + 1] == '?'))
		{
			expanded = get_expanded_value(line, &i, mini);
			result = str_append(result, expanded);
			free(expanded);
		}
		else
			result = append_char(result, line[i++]);
	}
	return (result);
}
