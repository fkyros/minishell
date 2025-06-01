/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_token_aux.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:43:45 by jorexpos          #+#    #+#             */
/*   Updated: 2025/06/01 16:43:46 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*detect_empty_literal(t_token_detect *tokens)
{
	char	*tok;

	if (tokens->input[*tokens->index] == '"'
		&& tokens->input[*tokens->index + 1] == '"')
	{
		*tokens->index += 2;
		tok = malloc(2);
		if (!tok)
			exit(EXIT_FAILURE);
		tok[0] = '\1';
		tok[1] = '\0';
		return (tok);
	}
	if (tokens->input[*tokens->index] == '\''
		&& tokens->input[*tokens->index + 1] == '\'')
	{
		*tokens->index += 2;
		tok = malloc(2);
		if (!tok)
			exit(EXIT_FAILURE);
		tok[0] = '\1';
		tok[1] = '\0';
		return (tok);
	}
	return (NULL);
}

char	*detect_double_redir(t_token_detect *tokens)
{
	if (tokens->input[*tokens->index] == '<'
		&& tokens->input[*tokens->index + 1] == '<')
	{
		*tokens->index += 2;
		return (ft_strdup("<<"));
	}
	if (tokens->input[*tokens->index] == '>'
		&& tokens->input[*tokens->index + 1] == '>')
	{
		*tokens->index += 2;
		return (ft_strdup(">>"));
	}
	return (NULL);
}

char	*detect_single_op(t_token_detect *tokens)
{
	char	op[2];

	if (tokens->input[*tokens->index] == '<'
		|| tokens->input[*tokens->index] == '>'
		|| tokens->input[*tokens->index] == '|')
	{
		op[0] = tokens->input[*tokens->index];
		op[1] = '\0';
		(*tokens->index)++;
		return (ft_strdup(op));
	}
	return (NULL);
}

int	process_quote_or_dollar(t_word_process *words)
{
	char		c;
	t_dollar	context;

	c = words->input[*words->index];
	if ((c == '\'' && *words->quote_st != STATE_DOUBLE)
		|| (c == '"' && *words->quote_st != STATE_SINGLE))
	{
		if (*words->quote_st == STATE_NONE)
			*words->flag_quoted = 1;
		*words->quote_st = quote_state_after(*words->quote_st, c);
		(*words->index)++;
		return (1);
	}
	if (c == '$' && *words->quote_st != STATE_SINGLE
		&& !is_after_heredoc(words->input, *words->index))
	{
		context.input = words->input;
		context.index = words->index;
		context.mini = words->mini;
		context.buf = words->buffer;
		context.pos = words->buf_pos;
		handle_dollar(&context);
		return (1);
	}
	return (0);
}

void	process_normal_char(t_word_process *words)
{
	words->buffer[*words->buf_pos] = words->input[*words->index];
	(*words->buf_pos)++;
	(*words->index)++;
}
