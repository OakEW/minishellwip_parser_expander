/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:32:57 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/07 17:30:26 by ywang2           ###   ########.fr       */
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

t_argv	*make_argv(t_token *token)
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
	if (!line[0])
		return (free (line), -1);
	token = lexer(line, env);
	if (!token && env->exit_s == 42)
		return (free_tokens(token), free (line), env->exit_s = 0, -1);
	if (!token && env->exit_s != 42)
		return (free_tokens(token), malloc_fail(env), 0);
	if (syntax_error(line, token, env))
		return (0);
	head = make_argv(token);
	if (!head)
		return (free_tokens(token), malloc_fail(env), 0);
	*out = head;
	free_tokens(token);
	return (1);
}
