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

void builtin_echo(char **args, t_mini *mini)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	echo_args(args, &newline, &i);
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[(i++) + 1])
			write(1, " ", 1);
	}
	if (newline)
		write(1, "\n", 1);
	mini->last_status = 0;
}

void	update_cd_vars(t_mini *mini)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;
	char	**new_env;

	oldpwd = ft_getenv("PWD", mini->our_env);
	new_env = add_var_to_env(mini->our_env, "OLDPWD", oldpwd);
	free(oldpwd);
	free_array(mini->our_env);
	mini->our_env = new_env;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		perror(BOLD RED"minishell error: pwd"RST);
	new_env = add_var_to_env(mini->our_env, "PWD", cwd);
	free_array(mini->our_env);
	mini->our_env = new_env;
	mini->last_status = 0;
}


void	builtin_cd(char **args, t_mini *mini)
{
	char	*home;
	int	error;

	error = 0;
	if (!args[1])
	{
		home = ft_getenv("HOME", mini->our_env);
		if (!home)
			ft_putstr_fd(BOLD RED"Minishell: cd: HOME not set\n"RST, STDOUT_FILENO);
		error = 1;
		if (safe_chdir(home) != 0)
			error = 1;
	}
	else
	{
		if (get_num_args(args) > 1)
		{
			ft_putstr_fd(BOLD RED"Minishell: cd: too many arguments\n"RST, STDOUT_FILENO);
			error = 1;
		}
		else if (safe_chdir(args[1]) != 0)
			error = 1;
	}
	if (error)
	{
		mini->last_status = GENERIC_ERROR;
		return ;
	}
	update_cd_vars(mini);
}

void builtin_pwd(char **args, t_mini *mini)
{
	char	cwd[PATH_MAX];

	if (args[1])
	{
		ft_putstr_fd("error: pwd: too many arguments\n", STDERR_FILENO);
		mini->last_status = GENERIC_ERROR;
		return ;
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		perror(BOLD RED"minishell error: pwd"RST);
	printf("%s\n", cwd);
	mini->last_status = 0;
}
