#include "../../inc/minishell.h"

#include <errno.h>
#include <string.h>
#include "../inc/minishell.h"

static void	exit_if_empty_command(char **argv)
{
	if (argv[0] && argv[0][0] == '\0')
	{
		fprintf(stderr, "'' : command not found\n");
		exit(127);
	}
	if (!argv[0])
		exit(EXIT_SUCCESS);
}

static void	exec_if_path(char **argv, char **envp)
{
	struct stat	st;

	if (stat(argv[0], &st) < 0)
	{
		fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
		exit(127);
	}
	if (S_ISDIR(st.st_mode))
	{
		fprintf(stderr, "%s: Is a directory\n", argv[0]);
		exit(126);
	}
	if (access(argv[0], X_OK) != 0)
	{
		fprintf(stderr, "%s: Permission denied\n", argv[0]);
		exit(126);
	}
	execve(argv[0], argv, envp);
	perror(argv[0]);
	exit(127);
}

static void	exec_from_env(char **argv, char **envp)
{
	char		*path;
	struct stat	st;

	path = search_command(argv[0], envp);
	if (!path)
	{
		fprintf(stderr, "%s: command not found\n", argv[0]);
		exit(127);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		fprintf(stderr, "%s: Is a directory\n", argv[0]);
		free(path);
		exit(126);
	}
	if (access(path, X_OK) != 0)
	{
		fprintf(stderr, "%s: Permission denied\n", argv[0]);
		free(path);
		exit(126);
	}
	execve(path, argv, envp);
	perror(argv[0]);
	free(path);
	exit(127);
}

void	exec_command(char **argv, char **envp)
{
	exit_if_empty_command(argv);
	if (strchr(argv[0], '/') != NULL)
		exec_if_path(argv, envp);
	else
		exec_from_env(argv, envp);
}




