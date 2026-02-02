#include "lexer.h"

int	exit_status;

void	free_argv(t_argv *head)
{
	t_argv	*tmp;
	int		i;

	while (head)
	{
		i = 0;
		tmp = head;
		head = head->next;
		if (tmp->argv)
		{
			while (tmp->argv[i])
			{
				free(tmp->argv[i]);
				i++;
			}
			free(tmp->argv);
		}
		free(tmp);
	}
}

size_t	ft_strlen(const char *str)
{
	int	x;

	x = 0;
	if (!str)
		return (0);
	while (str[x] != '\0')
		x++;
	return (x);
}

char	*ft_strdup(char *s)
{
	char	*dup;
	int		i;

	i = 0;
	dup = (char *) malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (dup == 0)
		return (0);
	while (s[i] != 0)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

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

t_token	*make_argv(t_argv **head, t_argv **curt, t_token *token, int *flag)
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

void trim_q(char *s)
{
	int	i;
	int	n;
	int	sq;
	int	dq;

	i = 0;
	n = 0;
	sq = 0;
	dq = 0;
	while (s[i])
	{
		if (s[i] == '\'' && !dq)
			sq = !sq;
		else if (s[i] == '\"' && !sq)
			dq = !dq;
		else
			s[n++] = s[i];
		i++;
	}
	s[n] = 0;
}


int	trim_expand(t_argv *curt, t_env *env)
{
	int		len;
	int		i;

	i = 0;
	while (curt->argv[i])
	{
		if (curt->type <= 2)
		{
			expander(&curt->argv[i], env);//		do not expand '$'
			trim_q(curt->argv[i]);
		}
		i++;
	}
	return (0);
}

t_argv	*make_expand(t_token *token, t_env*env)
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
		tmp = make_argv(&head, &current, tmp, &flag);
		trim_expand(current, env);
	}
	return (head);
}

int	op_mix(t_token_type x)
{
	if (x == PIPE || x == THEN || x == ELSE)
		return (1);
	else if (x == INPUT || x == OUTPUT || x == HEREDOC || x == APPEND)
		return (2);
	else if (x == PARENT_O)
		return (3);
	else if (x == PARENT_C)
		return (4);
	else
		return (0);
}

int	syntax_check(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	if (op_mix(token->type) == 1)
		return (258);
	while (tmp)
	{
		if (tmp->next == NULL && op_mix(tmp->type) > 0 && op_mix(tmp->type) < 3)
			return (258);
		if (op_mix(tmp->type) == 1 && tmp->next && op_mix(tmp->next->type) == 1)
			return (258);
		if (op_mix(tmp->type) == 1 && tmp->next && op_mix(tmp->next->type) == 4)
			return (258);
		if (op_mix(tmp->type) == 2 && tmp->next && op_mix(tmp->next->type) > 0)
			return (258);
		if (op_mix(tmp->type) == 3 && tmp->next && op_mix(tmp->next->type) == 1)
			return (258);
		if (op_mix(tmp->type) == 3 && tmp->next && op_mix(tmp->next->type) == 4)
			return (258);
		tmp = tmp->next;
	}
	return (0);
}

t_argv	*build_argv(char *line, t_env *env)
{
	t_token *token;
	t_argv *head;

	token = lexer(line);
	if (!token)
	{
		exit_status = 1;
		return (NULL);
	}
	if (precheck_line(line) != 0 || syntax_check(token) != 0)
	{
		exit_status = 258;
		free_tokens(token);
		return (write (2, "syntax error\n", 13), NULL);
	}
	head = make_expand(token, env);
	if (!head)
	{
		exit_status = 1;
		free_tokens(token);
		return (NULL);
	}
	free_tokens(token);				//tokens are freed here onwards
	return (head);
}




void	print_argv(t_argv *head)
{
	t_argv *tmp;

	int	x = 0;
	tmp = head;
	while (tmp)
	{
		int i = 0;
		printf("struct[%d] type: %d\n",x, tmp->type);
		while (i < tmp->argc)
		{
			printf("------->argv[%d]: {%s}\n", i, tmp->argv[i]);
			i++;
		}
		printf ("\n");
		x++;
		tmp = tmp->next;
	}
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
	t_argv *head;
	t_env	*env;

	env = init_env(envp);
	if (!env)
		return (0);
	signal(SIGQUIT, SIG_IGN);		//"ctrl -\"
	signal(SIGINT, handle_sigint);	// ctrl -C
	while (1)
	{
		line = readline("M-S $ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		head = build_argv(line, env);
		if(head)
		{
			print_argv(head);
			free_argv(head);							// free struct argv in main
		}
		free(line);
	}
	rl_clear_history();
	write(1, "Exit Mini_Shell\n", 17);
	// signal(SIGQUIT, SIG_DFL);
	return (0);
}
