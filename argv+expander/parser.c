/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:32:57 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/06 17:44:35 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	make_argv_helper(t_argv **head, t_argv **curt, t_token **t, int *flag)
{
	t_token			*start;
	t_argv			*new;

	if (!*t)
		return (2);
	start = *t;
	new = argv_init(*t);
	if (!new)
		return (0);
	if ((*t)->type == INPUT || (*t)->type == OUTPUT
		|| (*t)->type == APPEND || (*t)->type == HEREDOC)
			*flag = 1;
	new->argc = count_argc(t, flag);
	if (make_str(new, start) < 0)
		return (0);
	if (!*head)
		*head = new;
	else
		(*curt)->next = new;
	*curt = new;
	return (1);
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
		if (!make_argv_helper(&head, &current, &tmp, &flag))
			return (NULL);
	}
	return (head);
}

int	build_argv(char *line, t_env *env, t_argv **out)
{
	t_token	*token;
	t_argv	*head;

	*out = NULL;
	token = lexer(line);
	if (!token)
	{
		env->exit_s = 1;
		return (0);
	}
	if (syntax_error(line, token, env))
		return (0);
	head = make_argv(token, env);
	if (!head)
	{
		env->exit_s = ENOMEM;
		return (free_tokens(token), perror("malloc"), 0);
	}
	*out = head;
	free_tokens(token);
	return (1);
}
