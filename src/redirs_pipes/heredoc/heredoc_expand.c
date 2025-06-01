#include "../../../inc/minishell.h"

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

char	*expand_variable(const char *line, int *i, t_mini *mini)
{
	char	*name;
	char	*dollar_var;
	char	*value;

	if (line[*i] == '$' && line[*i + 1] == '?')
	{
		*i += 2;
		return (ft_itoa(mini->last_status));
	}
	(*i)++;
	name = extract_var_name(line, i);
	if (!name)
		return (ft_strdup(""));
	dollar_var = ft_strjoin("$", name);
	free(name);
	value = expand(dollar_var, mini);
	free(dollar_var);
	if (!value)
		return (ft_strdup(""));
	return (value);
}

static char	*get_expanded_value(const char *line, int *i, t_mini *mini)
{
	if (line[*i + 1] == '?')
		return (expand_status(i, mini));
	return (expand_variable(line, i, mini));
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
