#include "../inc/minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo") || 
            !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") || 
            !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") || 
            !ft_strcmp(cmd, "exit"));
}

void execute_builtin(t_command *cmd)
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
        return ;
    }
    // EXECUTE BUILTIN
    if (!ft_strcmp(cmd->argv[0], "cd"))
        builtin_cd(cmd->argv);
    else if (!ft_strcmp(cmd->argv[0], "echo"))
        builtin_echo(cmd->argv);
    else if (!ft_strcmp(cmd->argv[0], "pwd"))
        builtin_pwd();
    // RESTORE ORIGINAL FDS
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
}