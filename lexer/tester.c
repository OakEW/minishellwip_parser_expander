#include "lexer.h"

#include<stdio.h>

int print_tokens(char *line)
{

	t_token *head;
	t_token *tmp;

	head = lexer(line);
	tmp = head;
	while (tmp)
	{
		printf("Token type: %d, value: {%s}\n", tmp->type, tmp->value);
		tmp = tmp->next;
	}
	free_tokens(head);
	return 0;
}

int print_pwd(void)
{
	char *pwd;

	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}




void handle_sigint(int sig)	// ctrl -C
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	signal(SIGQUIT, SIG_IGN);		//"ctrl -\"
	signal(SIGINT, handle_sigint);	// ctrl -C
	if (argc != 1)
		exit (1);
	while (1)
	{
		line = readline("M-S $ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		if (strcmp(line, "pwd") == 0)
			print_pwd();
		else
			print_tokens(line);
		free(line);
	}
	rl_clear_history();
	write(1, "Exit Mini_Shell\n", 17);
	// signal(SIGQUIT, SIG_DFL);
	return (0);
}
