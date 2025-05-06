#include "../inc/minishell.h"

static char *get_next_token(const char *s, int *i, t_mini *mini)
{
	char	buf[4096];
	size_t	pos;
	int		state;
	char	c;
	char	op_str[2];

    while (s[*i] && is_whitespace(s[*i]))
        (*i)++;
    if (s[*i] == '<' && s[*i+1] == '<')
    {
        *i += 2;
        return (ft_strdup("<<"));
    }
    if (s[*i] == '>' && s[*i+1] == '>')
    {
        *i += 2;
        return (ft_strdup(">>"));
    }
    if (s[*i] == '<' || s[*i] == '>' || s[*i] == '|')
    {
        op_str[0] = s[*i];
		op_str[1] = '\0';
        (*i)++;
        return (ft_strdup(op_str));
    }
    pos = 0;
    state = STATE_NONE;
    ft_bzero(buf, sizeof(buf));
    while (s[*i] && (state != STATE_NONE || !is_whitespace(s[*i])))
    {
        c = s[*i];
        if ((c == '\'' && state != STATE_DOUBLE)
         || (c == '"' && state != STATE_SINGLE))
        {
            state = quote_state_after(state, c);
            (*i)++;
        }
        else if (c == '$' && state != STATE_SINGLE)
            handle_dollar(s, i, mini, buf, &pos);
        else
        {
            buf[pos++] = c;
            (*i)++;
        }
    }
    buf[pos] = '\0';
    if (state != STATE_NONE)
    {
        ft_putstr_fd("minishell: syntax error: unclosed quotes\n", STDERR_FILENO);
        return (NULL);
    }
    return (ft_strdup(buf));
}


int count_tokens(const char *str)
{
    int index = 0;
    int count = 0;

    while (str[index])
    {
        while (str[index] && is_whitespace(str[index]))
            index++;
        if (!str[index])
            break ;
        if ((str[index] == '<' && str[index+1] == '<') ||
            (str[index] == '>' && str[index+1] == '>'))
        {
            count++;
            index += 2;
            continue ;
        }
        if (str[index] == '<' || str[index] == '>' || str[index] == '|')
        {
            count++;
            index++;
            continue ;
        }
        count++;
        if (is_quote(str[index]))
            skip_quoted_section(str, &index, str[index]);
        else
            skip_unquoted_section(str, &index);
    }
    return (count);
}


char **parse_command(const char *cmd, int *token_count, t_mini *mini) 
{
    int index;
    int count;
    char **tokens;
	int	i;

	if (!check_unclosed_quotes(cmd))
    {
        ft_putstr_fd("minishell: syntax error: unclosed quotes\n", STDERR_FILENO);
        *token_count = 0;
        return (NULL);
    }
	index = 0;
	count = count_tokens(cmd);
	tokens = malloc((count + 1) * sizeof(char *));
	if (!cmd && !tokens)
	{
		free(tokens);
		return (NULL);
	}
    *token_count = count;
    index = 0;
    i = 0;
    while (i < count) {
        tokens[i] = get_next_token(cmd, &index, mini);
        i++;
    }
    tokens[i] = NULL;
    return (tokens);
}
