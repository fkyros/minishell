#include "../inc/minishell.h"

void print_matrix(char **matrix)
{
	int i = 0;
	while (matrix[i])
	{
		printf("%s\n", matrix[i]);
		i++;
	}
}

void free_matrix(char **matrix)
{
	int i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
}

int main(void)
{
	char 	*input;
	char	**command;

	while (1)
	{
		input = readline("$ ");
		command = ft_split(input, ' ');	
		free(input);
		print_matrix(command);
		free_matrix(command);
	}
	return (0);
}
