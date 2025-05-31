#include "../inc/minishell.h"

int	is_after_heredoc(const char *s, int idx)
{
	int	j;

	j = idx - 1;
	while (j >= 0 && is_whitespace(s[j]))
		j--;
	if (j < 1)
		return (0);
	if (s[j] == '<' && s[j - 1] == '<')
		return (1);
	return (0);
}

char	*consume_special(const char *s, int *i)
{
	t_token_detect	tokens;
	char			*tok;

	tokens.input = s;
	tokens.index = i;
	tok = detect_empty_literal(&tokens);
	if (tok)
		return (tok);
	tok = detect_double_redir(&tokens);
	if (tok)
		return (tok);
	return (detect_single_op(&tokens));
}

static int	process_word_loop(t_word_process *words)
{
	while (words->input[*words->index]
		&& (*words->quote_st != STATE_NONE
			|| !is_whitespace(words->input[*words->index])))
	{
		if (*words->quote_st == STATE_NONE
			&& (words->input[*words->index] == '<'
				|| words->input[*words->index] == '>'
				|| words->input[*words->index] == '|'))
			break ;
		if (process_quote_or_dollar(words))
			continue ;
		process_normal_char(words);
	}
	words->buffer[*words->buf_pos] = '\0';
	return (0);
}

static char	*handle_word_result(t_word_process *words)
{
	char	op_marker[3];

	if (*words->quote_st != STATE_NONE)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n",
			STDERR_FILENO);
		return (NULL);
	}
	if (*words->flag_quoted && *words->buf_pos == 1
		&& (words->buffer[0] == '|' || words->buffer[0] == '<'
			|| words->buffer[0] == '>'))
	{
		op_marker[0] = '\x02';
		op_marker[1] = words->buffer[0];
		op_marker[2] = '\0';
		return (ft_strdup(op_marker));
	}
	return (ft_strdup(words->buffer));
}

char	*consume_word(const char *s, int *i, t_mini *mini)
{
	char				buf[4096];
	size_t				pos;
	int					state;
	int					was_quoted;
	t_word_process		words;

	pos = 0;
	state = STATE_NONE;
	was_quoted = 0;
	ft_bzero(buf, sizeof(buf));
	words.input = s;
	words.index = i;
	words.mini = mini;
	words.buffer = buf;
	words.buf_pos = &pos;
	words.quote_st = &state;
	words.flag_quoted = &was_quoted;
	process_word_loop(&words);
	return (handle_word_result(&words));
}
