/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:42:30 by jorexpos          #+#    #+#             */
/*   Updated: 2025/06/01 16:42:31 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	unescape_quoted_ops(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (argv[i][0] == '\x02')
			ft_memmove(argv[i], argv[i] + 1, ft_strlen(argv[i]));
		i++;
	}
}

static void	exit_if_empty_command(char **argv)
{
	if (argv[0] && argv[0][0] == '\0')
		return ;
	if (!argv[0])
		return ;
}

static void	exec_if_path(char **argv, char **envp)
{
	struct stat	st;

	if (stat(argv[0], &st) < 0)
		return ;
	if (S_ISDIR(st.st_mode))
	{
		errno = EISDIR;
		return ;
	}
	if (access(argv[0], X_OK) != 0)
	{
		errno = EACCES;
		return ;
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(argv[0], argv, envp);
	return ;
}

static void	exec_from_env(char **argv, char **envp)
{
	char		*path;
	struct stat	st;

	path = search_command(argv[0], envp);
	if (!path)
		return ;
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		free(path);
		return ;
	}
	if (access(path, X_OK) != 0)
	{
		free(path);
		return ;
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(path, argv, envp);
	free(path);
	return ;
}

void	exec_command(char **argv, char **envp)
{
	unescape_quoted_ops(argv);
	exit_if_empty_command(argv);
	if (!argv[0])
		return ;
	if (ft_strchr(argv[0], '/') != NULL)
		exec_if_path(argv, envp);
	else
		exec_from_env(argv, envp);
}
