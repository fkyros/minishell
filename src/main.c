#include "../inc/minishell.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static int *get_readline_flag(void)
{
    static int in_readline = 0;

    return (&in_readline);
}

void signal_handler(int sig)
{
    int *in_readline;
    
    in_readline = get_readline_flag();
    if (sig == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        if (*in_readline)
        {
            rl_replace_line("", 0);
            rl_on_new_line();
            rl_redisplay();
        }
    }
    else if (sig == SIGWINCH)
    {
        if (*in_readline)
            rl_redisplay();
    }
}

int handle_ctrl_l(int count, int key)
{
    (void)count;
    (void)key;

    write(STDOUT_FILENO, "\033[H\033[2J", 7);

    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
    return (0);
}

void setup_signals(void)
{
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGWINCH, signal_handler);
    rl_bind_key('\f', handle_ctrl_l);
}

char *build_prompt(void)
{
    char *cwd;
    char *base;
    char *prompt;
    char *tmp;

    cwd = get_cwd();
    if (cwd)
    {
        tmp = ft_strjoin(BOLD GREEN, cwd);
        free(cwd);
        base = ft_strjoin(tmp, " > " RST);
        free(tmp);
    }
    else
        base = ft_strdup(BOLD GREEN "minishell > " RST);
    prompt = ft_strjoin(base, BOLD PINK "funny shell > " RST);
    free(base);
    return (prompt);
}

t_mini *init_shell(char **env)
{
    t_mini *mini = malloc(sizeof(t_mini));
    if (!mini)
        return NULL;

    mini->our_env = init_env(env);
    mini->last_status = 0;
    return mini;
}

int run_prompt_loop(t_mini *mini)
{
    t_parse_result parse_result;
    char *line;
    char *prompt;
    int *in_readline = get_readline_flag();

    while (1)
    {
        prompt = build_prompt();
        *in_readline = 1;
        line = readline(prompt);
        *in_readline = 0;
        free(prompt);
        if (!line)
        {
            printf(PINK BOLD"\nMinishell: exiting!\n"RST);
            return (0);
        }
        if (*line)
        {
            add_history(line);
            parse_result = parse_commands(line, mini->our_env);
            if (parse_result.cmd_count > 0)
                execute_pipeline(&parse_result, mini);
            free_commands(&parse_result);
        }
        free(line);
    }
}

int main(int argc, char **argv, char **env)
{
    t_mini *mini;

    (void)argc;
    (void)argv;
    setup_signals();
    print_banner();

    mini = init_shell(env);
    if (!mini)
        return (1);
    run_prompt_loop(mini);
    free_array(mini->our_env);
    free(mini);
    return (EXIT_SUCCESS);
}
