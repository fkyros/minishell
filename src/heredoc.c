#include "../inc/minishell.h"

int process_heredoc(t_command *cmd) {
    int pipe_fd[2];
    char *line;

    if (!cmd->heredoc) return 0;

    if (pipe(pipe_fd)) {
        perror("minishell: pipe");
        return (-1);
    }
    while (1) {
        line = readline("> ");
        if (!line) {
            ft_putstr_fd(BOLD RED"minishell: HEREDOC delimited by EOF\n", STDERR_FILENO);
            break ;
        }
        if (ft_strcmp(line, cmd->heredoc) == 0)
        {
            free(line);
            break ;
        }
        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    close(pipe_fd[1]);
    cmd->heredoc_fd = pipe_fd[0];
    return (0);
}