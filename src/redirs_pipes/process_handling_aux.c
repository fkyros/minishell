#include "../inc/minishell.h"

int	*alloc_and_zero_exit_codes(int count)
{
	int		*arr;
	int		i;

	arr = malloc(sizeof(int) * count);
	if (!arr)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < count)
	{
		arr[i] = 0;
		i++;
	}
	return (arr);
}

int	find_pid_index(pid_t *pids, pid_t pid, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pids[i] == pid)
			return (i);
		i++;
	}
	return (-1);
}

void	update_exit_code(int *exit_codes, int idx, int status)
{
	int	sig;

	if (WIFEXITED(status))
		exit_codes[idx] = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGPIPE)
			exit_codes[idx] = 0;
		else
			exit_codes[idx] = 128 + sig;
	}
}

void print_error_for_child(char *cmdname, int code)
{
    int          is_path;
    struct stat  st;

    if (cmdname == NULL)
        return ;
    is_path = (ft_strchr(cmdname, '/') != NULL);
    if (code == 127)
    {
        ft_putstr_fd(cmdname, STDERR_FILENO);
        if (is_path)
        {
            if (stat(cmdname, &st) < 0)
                ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
            else if (S_ISDIR(st.st_mode))
                ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
            else
                ft_putstr_fd(": command not found\n", STDERR_FILENO);
        }
        else
            ft_putstr_fd(": command not found\n", STDERR_FILENO);
    }
    else if (code == 126 && is_path)
    {
        ft_putstr_fd(cmdname, STDERR_FILENO);
        if (stat(cmdname, &st) == 0 && S_ISDIR(st.st_mode))
            ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
        else
            ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
    }
}

void print_all_child_errors(t_parse_result *res, int *exit_codes)
{
    int   i;
    int   code;
    char *cmdname;

    i = 0;
    while (i < res->cmd_count)
    {
        code    = exit_codes[i];
        cmdname = res->commands[i].argv[0];
        print_error_for_child(cmdname, code);
        i++;
    }
}
