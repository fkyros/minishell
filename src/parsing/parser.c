#include "../inc/minishell.h"

static int is_after_heredoc(const char *s, int idx)
{
    int  j;

    j = idx - 1;
    while (j >= 0 && is_whitespace(s[j]))
        j--;
    if (j < 1)
        return (0);
    if (s[j] == '<' && s[j - 1] == '<')
        return (1);
    return (0);
}

static char *get_next_token(const char *s, int *i, t_mini *mini)
{
    char    buf[4096];
    size_t  pos;
    int     state;
    char    c;
    char    op_str[3];
    char    *tok;
    int     was_quoted = 0;

    while (s[*i] && is_whitespace(s[*i]))
        (*i)++;
    if (s[*i] == '"' && s[*i + 1] == '"')
    {
        *i += 2;
        tok = malloc(2);
        if (!tok) exit(EXIT_FAILURE);
        tok[0] = '\1';
        tok[1] = '\0';
        return tok;
    }
    if (s[*i] == '\'' && s[*i + 1] == '\'')
    {
        *i += 2;
        tok = malloc(2);
        if (!tok) exit(EXIT_FAILURE);
        tok[0] = '\1';
        tok[1] = '\0';
        return (tok);
    }
    if (s[*i] == '<' && s[*i + 1] == '<')
    {
        *i += 2;
        return (ft_strdup("<<"));
    }
    if (s[*i] == '>' && s[*i + 1] == '>')
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
    pos   = 0;
    state = STATE_NONE;
    ft_bzero(buf, sizeof(buf));

    while (s[*i] && (state != STATE_NONE || !is_whitespace(s[*i])))
    {
        if (state == STATE_NONE && (s[*i] == '<' || s[*i] == '>' || s[*i] == '|'))
            break ;

        c = s[*i];
        if ((c == '\'' && state != STATE_DOUBLE) || (c == '"' && state != STATE_SINGLE))
        {
            if (state == STATE_NONE)
                was_quoted = 1;
            state = quote_state_after(state, c);
            (*i)++;
        }
        else if (c == '$' && state != STATE_SINGLE && !is_after_heredoc(s, *i))
        {
            handle_dollar(s, i, mini, buf, &pos);
        }
        else
        {
            buf[pos++] = c;
            (*i)++;
        }
    }
    buf[pos] = '\0';
    if (state != STATE_NONE)
    {
        ft_putstr_fd("minishell: syntax error: unclosed quotes\n",
                     STDERR_FILENO);
        return (NULL);
    }
    if (was_quoted && pos == 1 && (buf[0] == '|' 
        || buf[0] == '<' || buf[0] == '>'))
    {
        op_str[0] = '\x02';
        op_str[1] = buf[0];
        op_str[2] = '\0';
        tok = ft_strdup(op_str);
    }
    else
        tok = ft_strdup(buf);
    return (tok);
}


int count_tokens(const char *str)
{
    int index;
    int count;

    index = 0;
    count = 0;
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
    int   index;
    int   used;
    int   cap;
    char **tokens;
    char  *tok;

    index = 0;
    used = 0;
    cap = 4;
    tokens = malloc(cap * sizeof(char *));
    if (!tokens || !check_unclosed_quotes(cmd))
    {
        ft_putstr_fd("minishell: syntax error: unclosed quotes\n",
                     STDERR_FILENO);
        *token_count = 0;
        return (NULL);
    }
    while (index < (int)ft_strlen(cmd) &&
           (tok = get_next_token(cmd, &index, mini)) != NULL)
    {
        if (tok[0] == '\1')
            tok[0] = '\0';
        else if (tok[0] == '\0')
        {
            free(tok);
            continue ;
        }
        if (used + 1 >= cap)
            tokens = realloc(tokens, (cap *= 2) * sizeof(char *));
        tokens[used++] = tok;
    }
    tokens[used] = NULL;
    *token_count = used;
    return (tokens);
}

