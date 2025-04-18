#include "../inc/minishell.h"

// ECHO, CD, PWD, EXPORT
static void    echo_args(char **args, int *newline, int *i)
{
    int parsing_flags;
    int j;

    parsing_flags = 1;
    while (args[*i] && parsing_flags)
    {
        if (args[*i][0] == '-' && args[*i][1] == 'n')
        {
            // CHECK IF IT'S A VALID STRING
            j = 2;
            while (args[*i][j] == 'n')
                j++;
            // IF IT'S ONLY THE FLAG
            if (args[(*i)++][j] == '\0')
                *newline = 0;
            else  // NOT A VALID FLAG, STOP PROCESSING THEM
                parsing_flags = 0;
        }
        else
            parsing_flags = 0;
    }
}

void builtin_echo(char **args)
{
    int i = 1;
    int newline = 1;
    // PROCESS FLAGS (EVEN COMBINED ONES LIKE -nnn)
    echo_args(args, &newline, &i);
    // PRINT THE REST
    while (args[i])
    {
        write(1, args[i], ft_strlen(args[i]));
        if (args[(i++) + 1])
            write(1, " ", 1);
    }
    if (newline)
        write(1, "\n", 1);
}

// TODO: change for ft_getenv, and change internal PWD and OLDPWD env variables
void builtin_cd(char **args)
{
    char *home;

    if (!args[1])
    {
        home = getenv("HOME");
        if (!home)
            ft_putstr_fd(BOLD RED"Minishell: cd: HOME not set\n"RST, 2);
        if (chdir(home) != 0)
            perror(BOLD RED"minishell error: cd"RST);
    }
    else
    {
        if (chdir(args[1]) != 0)
            perror(BOLD RED"minishell error: cd"RST);
    }
}

void builtin_pwd(void)
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
        perror(BOLD RED"minishell error: pwd"RST);
    printf("%s\n", cwd);
}
