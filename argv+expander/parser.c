#include "argv_env.h"

t_argv	*argv_init(t_token *token)
{
	t_argv	*argv;

	argv = malloc(sizeof(t_argv));
	if (!argv)
		return (NULL);
	argv->argc = 0;
	argv->next = NULL;
	argv->type = token->type;
	if (argv->type <= 2)
		argv->type = WORD;
	return (argv);
}

int	count_argc(t_token **token, int *flag)
{
	int	n;

	n = 0;
	if (!*token)
		return (0);
	if ((*token)->type > 2)
	{
		(*token) = (*token)->next;
		return (1);
	}
	if (*flag > 0 && (*token)->type <= 2)
	{
		(*token) = (*token)->next;
		*flag = -1;
		return (1);
	}
	while (*token && (*token)->type <= 2)
	{
		n++;
		(*token) = (*token)->next;
	}
	return (n);
}

int	make_str(t_argv *argv, t_token *token)
{   
	int	n;

	n = 0;
	argv->argv = malloc(sizeof(char *) * (argv->argc + 1));
	if (!argv->argv)
		return (-1);
	while (n < argv->argc)
	{
		argv->argv[n++] = ft_strdup(token->value);
		token = token->next;
	}
	argv->argv[n] = 0;
	return (0);
}

t_token	*make_argv_helper(t_argv **head, t_argv **curt, t_token *token, int *flag)
{
	t_token			*start;
	t_argv			*new;

	if (!token)
		return (NULL);
	start = token;
	new = argv_init(token);
	if(!new)
		return (NULL);
	if (token->type == INPUT || token->type == OUTPUT || token->type == APPEND || token->type == HEREDOC)
			*flag = 1;
	new->argc = count_argc(&token, flag);
	if (make_str(new, start) < 0)
		return (NULL);
	if (!*head)
		*head = new;
	else
		(*curt)->next = new;
	*curt = new;
	return (token);
}

t_argv	*make_argv(t_token *token, t_env*env)
{
	t_argv	*current;
	t_argv	*head;
	t_token	*tmp;
	int		flag;

	current = NULL;
	head = NULL;
	tmp = token;
	flag = -1;
	if (!token)
		return (NULL);
	while (tmp)
	{
		tmp = make_argv_helper(&head, &current, tmp, &flag);
	}
	return (head);
}

int	build_argv(char *line, t_env *env, t_argv **out)
{
	t_token *token;
	t_argv	*head;

	*out = NULL;
	token = lexer(line);
	if (!token)
	{
		env->exit_s = 1;
		return (0);
	}
	if (precheck_line(line) != 0 || syntax_check(token) != 0)
	{
		env->exit_s = 2;
		free_tokens(token);
		return (write (2, RED"syntax error\n"RESET, 22), 0);
	}
	head = make_argv(token, env);
	if (!head)
	{
		env->exit_s = 1;
		return (free_tokens(token), 0);
	}
	*out = head;
	free_tokens(token);				//tokens are freed here
	return (1);
}

// // for test


// int	print_argv(t_argv *head, t_env *env)
// {
// 	t_argv *tmp;

// 	int	x = 0;
// 	tmp = head;
// 	while (tmp)
// 	{
// 		int i = 0;
// 		while (i < tmp->argc)
// 		{
// 			trim_expand(tmp, env);
// 			printf(BLUE"t_argv[%d] type:%d",x, tmp->type);
// 			printf(GREEN" -->argv[%d]: {%s}\n"RESET, i, tmp->argv[i]);
// 			i++;
// 		}
// 		printf ("\n");
// 		x++;
// 		tmp = tmp->next;
// 	}
// 	return (0);
// }

// void handle_sigint(int sig)	// ctrl -C
// {
// 	(void)sig;
// 	write(1, "\n", 1);
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	rl_redisplay();
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	char	*line;
// 	t_argv *head;
// 	t_env	*env;

// 	env = init_env(envp);
// 	if (!env)
// 		return (0);
// 	signal(SIGQUIT, SIG_IGN);		//"ctrl -\"
// 	signal(SIGINT, handle_sigint);	// ctrl -C
// 	while (1)
// 	{
// 		line = readline(GREEN"M_S->"RESET);
// 		if (!line)
// 			break ;
// 		if (*line)
// 			add_history(line);
// 		if(build_argv(line, env, &head))
// 		{
// 			print_argv(head, env);
// 			env->exit_s = 0;
// 			free_argv(head);						// free struct argv in main
// 		}
// 		free(line);
// 	}
// 	rl_clear_history();
// 	free_env(env);
// 	write(1, YELLOW"Exit Mini_Shell\n"RESET, 25);
// 	// signal(SIGQUIT, SIG_DFL);
// 	return (0);
// }
