#include "../inc/minishell.h"

void    *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
    void    *new_ptr;
    size_t  temp_size;

    if (new_size == 0)
    {
        free(ptr);
        return (NULL);
    }
    if (!ptr)
        return (malloc(new_size));
    new_ptr = malloc(new_size);
    if (!new_ptr)
        return (NULL);
    if (old_size > 0)
    {
        if (old_size < new_size)
            temp_size = old_size;
        else
            temp_size = new_size;
        ft_memcpy(new_ptr, ptr, temp_size);
        if (new_size > old_size)
            ft_bzero((char *)new_ptr + old_size, new_size - old_size);
    }
    free(ptr);
    return (new_ptr);
}

void free_commands(t_parse_result *result) 
{
    int i;

    i = 0;
    if (!result) 
        return ;
    if (result->args) 
        free(result->args);
    if (result->original_tokens) 
    {
        while (i < result->token_count)
            free(result->original_tokens[i++]);
        free(result->original_tokens);
    }
    free(result->commands);
    result->commands = NULL;
    result->args = NULL;
    result->original_tokens = NULL;
    result->cmd_count = 0;
    result->token_count = 0;
}