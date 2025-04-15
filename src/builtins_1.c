#include "../inc/minishell.h"

// ECHO, CD, PWD, EXPORT
int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo") || 
            !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") || 
            !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") || 
            !ft_strcmp(cmd, "exit"));
}

void execute_builtin(t_command *cmd, char **our_env)
{
	// SAVE ORIGINAL FD SO WE CAN RESTORE THEM LATER
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);

	if (cmd->heredoc_fd != -1) 
	{
        close(cmd->heredoc_fd);
        cmd->heredoc_fd = -1;
    }
    // APPLY REDIRECTIONS
    if (apply_redirections(cmd) != 0)
    {
        close(original_stdin);
        close(original_stdout);
        return;
    }
    // EXECUTE BUILTIN
    if (!ft_strcmp(cmd->argv[0], "cd"))
        builtin_cd(cmd->argv);
    else if (!ft_strcmp(cmd->argv[0], "echo"))
        builtin_echo(cmd->argv);
    else if (!ft_strcmp(cmd->argv[0], "pwd"))
        builtin_pwd();
    else if (!ft_strcmp(cmd->argv[0], "env"))
        builtin_env(our_env);
    else if (!ft_strcmp(cmd->argv[0], "export"))
        builtin_export(cmd->argv, our_env);

    // RESTORE ORIGINAL FDS
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
}

void builtin_echo(char **args)
{
    int i = 1;
    int newline = 1;
    int parsing_flags = 1;

    // PROCESS FLAGS (EVEN COMBINED ONES LIKE -nnn)
    while (args[i] && parsing_flags)
    {
        if (args[i][0] == '-' && args[i][1] == 'n')
        {
            // CHECK IF IT'S A VALID STRING
            int j = 2;
            while (args[i][j] == 'n')
                j++;
            if (args[i++][j] == '\0')  // IF IT'S ONLY THE FLAG
                newline = 0;
            else  // NOT A VALID FLAG, STOP PROCESSING THEM
                parsing_flags = 0;
        }
        else
            parsing_flags = 0;
    }
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
            perror("cd");
    }
    else
    {
        if (chdir(args[1]) != 0)
            perror("cd");
    }
}

void builtin_pwd(void)
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
        perror("pwd");
    printf("%s\n", cwd);
}
