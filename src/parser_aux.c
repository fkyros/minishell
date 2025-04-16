#include "../inc/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
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

int is_operator(const char *token)
{
    if (!token)
        return (0);
        
    return (ft_strcmp(token, "|") == 0 ||
           ft_strcmp(token, ">") == 0 ||
           ft_strcmp(token, ">>") == 0 ||
           ft_strcmp(token, "<") == 0 ||
           ft_strcmp(token, "<<") == 0);
}
