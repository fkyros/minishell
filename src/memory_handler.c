#include "minishell.h"

void free_commands(t_parse_result *result) 
{
    int i;

    i = 0;
    if (!result) return;
    // WE FREE THE args ARRAY
    if (result->args) {
        free(result->args);
    }
    // WE FREE THE ORIGINAL TOKENS (SAVED FOR PRECAUTION)
    if (result->original_tokens) {
        // WE FREE EVERY INDIVIDUAL STRING ON THERE
        while (i < result->token_count)
            free(result->original_tokens[i++]);
        // WE FREE THE POINTER TO THE ARRAY ITSELF
        free(result->original_tokens);
    }
    // WE FREE THE COMMANDS ARRAY
    free(result->commands);
    // AND RESET ALL OF IT'S VALUES
    result->commands = NULL;
    result->args = NULL;
    result->original_tokens = NULL;
    result->cmd_count = 0;
    result->token_count = 0;
}