#include "../inc/minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    return (!ft_strcmp(cmd, "cd") || 
            !ft_strcmp(cmd, "echo") || 
            !ft_strcmp(cmd, "pwd") || 
            !ft_strcmp(cmd, "export") || 
            !ft_strcmp(cmd, "unset") || 
            !ft_strcmp(cmd, "env") || 
            !ft_strcmp(cmd, "exit"));
}

void execute_builtin(t_command *cmd, int apply_redirects, t_mini *mini)
{
    int saved_stdin;
    int saved_stdout;

    saved_stdin = -1;
    saved_stdout = -1;
    if (apply_redirects)
    {
        saved_stdin = dup(STDIN_FILENO);
        saved_stdout = dup(STDOUT_FILENO);
        if (saved_stdin == -1 || saved_stdout == -1)
        {
            perror("minishell: dup");
            return ;
        }
        if (apply_redirections(cmd) != 0)
        {
            close(saved_stdin);
            close(saved_stdout);
            return ;
        }
    }
    if (!ft_strcmp(cmd->argv[0], "cd"))
        builtin_cd(cmd->argv);
    else if (!ft_strcmp(cmd->argv[0], "echo"))
        builtin_echo(cmd->argv);
    else if (!ft_strcmp(cmd->argv[0], "pwd"))
        builtin_pwd();
    else if (!ft_strcmp(cmd->argv[0], "env"))
        builtin_env(mini->our_env);
    else if (!ft_strcmp(cmd->argv[0], "export"))
        builtin_export(cmd->argv, mini);
    if (apply_redirects)
    {
        if (dup2(saved_stdin, STDIN_FILENO) == -1)
            perror("minishell: dup2");
        if (dup2(saved_stdout, STDOUT_FILENO) == -1)
            perror("minishell: dup2");
        close(saved_stdin);
        close(saved_stdout);
    }
}