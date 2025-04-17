#include "../inc/minishell.h"

void add_redirect(t_command *cmd, enum e_redirect_type type, 
                        char *filename, char *heredoc_eof)
{
    t_redirect *new_redirs;
    int new_count;

    new_count = cmd->redir_count + 1;
    new_redirs = ft_realloc(cmd->redirs, 
                          cmd->redir_count * sizeof(t_redirect),
                          new_count * sizeof(t_redirect));
    if (!new_redirs)
        return ;
    new_redirs[cmd->redir_count] = (t_redirect){
        .type = type,
        .filename = filename,
        .heredoc_eof = heredoc_eof
    };
    cmd->redirs = new_redirs;
    cmd->redir_count = new_count;
}

void redirect_input_heredoc(t_command *cmd)
{
    int i;

    if (!cmd)
        return ;
    if (cmd->heredoc_fd != -1)
    {
        if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
            perror("minishell: dup2");
        close(cmd->heredoc_fd);
        cmd->heredoc_fd = -1;
        return ;
    }
    i = cmd->redir_count - 1;
    if (!cmd->redirs || cmd->redir_count <= 0)
        return ;
    while (i >= 0)
    {
        if (cmd->redirs[i].type == in && cmd->redirs[i].filename)
        {
            int fd = open(cmd->redirs[i].filename, O_RDONLY);
            if (fd == -1)
            {
                perror("minishell: open");
                continue ;
            }

            if (dup2(fd, STDIN_FILENO) == -1)
                perror("minishell: dup2");
            close(fd);
            return ;
        }
        i--;
    }
}

void redirect_output(t_command *cmd)
{
    int last_out_fd;
    int *created_fds;
    int fd_count;
    int i;
    int flags;
    int fd;

    last_out_fd = -1;
    created_fds = NULL;
    fd_count = 0;
    i = 0;
    if (!cmd || !cmd->redirs || cmd->redir_count <= 0)
        return ;
    created_fds = malloc(cmd->redir_count * sizeof(int));
    if (!created_fds) 
    {
        perror("minishell: malloc");
        return ;
    }

    while (i < cmd->redir_count)
    {
        if ((cmd->redirs[i].type == out || cmd->redirs[i].type == append) && 
            cmd->redirs[i].filename)
        {
            flags = O_WRONLY | O_CREAT;
            if (cmd->redirs[i].type == out)
                flags = flags + O_TRUNC;
            else 
                flags = flags + O_APPEND;
            fd = open(cmd->redirs[i].filename, flags, 0644);
            if (fd == -1) 
            {
                perror("minishell: open");
                continue ;
            }
            created_fds[fd_count++] = fd;
            if (last_out_fd != -1)
                close(last_out_fd);
            last_out_fd = fd;
        }
        i++;
    }
    if (last_out_fd != -1) 
    {
        if (dup2(last_out_fd, STDOUT_FILENO) == -1)
            perror("minishell: dup2");
        close(last_out_fd);
    }
    i = 0;
    while (i < fd_count)
    {
        if (created_fds[i] != last_out_fd)
            close(created_fds[i]);
        i++;
    }
    free(created_fds);
}

int apply_redirections(t_command *cmd)
{
    int saved_stdin;
    int saved_stdout;
    
    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    redirect_input_heredoc(cmd);
    redirect_output(cmd);
    if (dup2(STDIN_FILENO, STDIN_FILENO) == -1 || 
        dup2(STDOUT_FILENO, STDOUT_FILENO) == -1)
    {
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
        return (-1);
    }
    close(saved_stdin);
    close(saved_stdout);
    return (0);
}

void setup_input(t_command *cmd, int prev_pipe_fd)
{
    int i;
    int fd;

    if (cmd->heredoc_fd != -1) 
    {
        dup2(cmd->heredoc_fd, STDIN_FILENO);
        close(cmd->heredoc_fd);
        cmd->heredoc_fd = -1;
        return ;
    }
    i = cmd->redir_count - 1;
    while (i >= 0)
    {
        if (cmd->redirs[i].type == in) 
        {
            fd = open(cmd->redirs[i].filename, O_RDONLY);
            if (fd == -1) 
            {
                perror("minishell: open");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            return ;
        }
        i--;
    }
    if (!cmd->is_first) 
    {
        dup2(prev_pipe_fd, STDIN_FILENO);
    }
}

void setup_output(t_command *cmd, int (*pipe_fd)[2])
{
    int last_out;
    int i;
    int flags;
    int fd;

    last_out = -1;
    i = 0;
    while (i < cmd->redir_count)
    {
        if (cmd->redirs[i].type == out || cmd->redirs[i].type == append) 
        {
            flags = O_WRONLY | O_CREAT;
            if (cmd->redirs[i].type == out)
                flags = flags + O_TRUNC;
            else 
                flags = flags + O_APPEND;
            
            fd = open(cmd->redirs[i].filename, flags, 0644);
            if (fd == -1) 
            {
                perror("minishell: open");
                exit(1);
            }
            if (last_out != -1)
                close(last_out);
            last_out = fd;
        }
        i++;
    }
    if (last_out != -1) 
    {
        dup2(last_out, STDOUT_FILENO);
        close(last_out);
    }
    else if (cmd->pipe_out)
        dup2((*pipe_fd)[1], STDOUT_FILENO);
    close((*pipe_fd)[0]);
    close((*pipe_fd)[1]);
}