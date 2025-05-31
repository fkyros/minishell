#include "../inc/minishell.h"

int	quote_state_after(int state, char quote)
{
	if (quote == '\'' && state != STATE_DOUBLE)
	{
		if (state == STATE_SINGLE)
			return (STATE_NONE);
		return (STATE_SINGLE);
	}
	if (quote == '\"' && state != STATE_SINGLE)
	{
		if (state == STATE_DOUBLE)
			return (STATE_NONE);
		return (STATE_DOUBLE);
	}
	return (state);
}

void	append_to_buf(char *buf, size_t *pos, const char *src, size_t len)
{
	if (*pos + len > 4095)
		len = 4095 - *pos;
	ft_memcpy(buf + *pos, src, len);
	*pos += len;
}

void	handle_dollar(t_dollar *context)
{
	char	*var;
	size_t	len;

	var = expand_variable(context->input, context->index, context->mini);
	if (!var)
		return ;
	len = ft_strlen(var);
	append_to_buf(context->buf, context->pos, var, len);
	free(var);
}
