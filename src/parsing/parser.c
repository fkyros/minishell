#include "../inc/minishell.h"

char	*get_next_token(const char *s, int *i, t_mini *mini)
{
	char	*tok;

	while (s[*i] && is_whitespace(s[*i]))
		(*i)++;
	tok = consume_special(s, i);
	if (tok)
		return (tok);
	return (consume_word(s, i, mini));
}

int	count_tokens(const char *str)
{
	int	op_count;
	int	index;
	int	count;

	index = 0;
	count = 0;
	while (str[index])
	{
		while (str[index] && is_whitespace(str[index]))
			index++;
		if (!str[index])
			break ;
		op_count = handle_operator_tokens(str, &index);
		if (op_count)
		{
			count += op_count;
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

static char	**allocate_and_check(const char *cmd, int *token_count)
{
	char	**tokens;
	int		cap;

	cap = 4;
	tokens = malloc(cap * sizeof(char *));
	if (!tokens)
	{
		*token_count = 0;
		return (NULL);
	}
	if (!check_unclosed_quotes(cmd))
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n",
			STDERR_FILENO);
		free(tokens);
		*token_count = 0;
		return (NULL);
	}
	return (tokens);
}

static int	process_token(char *tok, char ***tokens, int *cap, int *used)
{
	if (tok[0] == '\1')
		tok[0] = '\0';
	else if (tok[0] == '\0')
	{
		free(tok);
		return (0);
	}
	if (*used + 1 >= *cap)
	{
		*cap *= 2;
		*tokens = realloc(*tokens, *cap * sizeof(char *));
	}
	(*tokens)[(*used)++] = tok;
	return (1);
}

char	**parse_command(const char *cmd, int *token_count, t_mini *mini)
{
	int		index;
	int		used;
	int		cap;
	char	**tokens;
	char	*tok;

	used = 0;
	cap = 4;
	tokens = allocate_and_check(cmd, token_count);
	if (!tokens)
		return (NULL);
	index = 0;
	while (index < (int)ft_strlen(cmd))
	{
		tok = get_next_token(cmd, &index, mini);
		if (tok == NULL)
			break ;
		process_token(tok, &tokens, &cap, &used);
	}
	tokens[used] = NULL;
	*token_count = used;
	return (tokens);
}
