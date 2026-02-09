/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:33:02 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/09 14:50:37 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	o_p(t_token_type x)
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
	if (o_p(token->type) == 1)
		return ('|');
	while (tmp)
	{
		if (tmp->next == NULL && o_p(tmp->type) > 0 && o_p(tmp->type) < 3)
			return ((int)tmp->value[0]);
		if (o_p(tmp->type) == 1 && tmp->next && o_p(tmp->next->type) == 1)
			return ('|');
		if (o_p(tmp->type) == 1 && tmp->next && o_p(tmp->next->type) == 4)
			return (')');
		if (o_p(tmp->type) == 2 && tmp->next && o_p(tmp->next->type) > 0)
			return ((int)tmp->next->value[0]);
		if (o_p(tmp->type) == 3 && tmp->next && o_p(tmp->next->type) == 1)
			return ((int)tmp->next->value[0]);
		if (o_p(tmp->type) == 3 && tmp->next && o_p(tmp->next->type) == 4)
			return ((int)tmp->next->value[0]);
		if (tmp->next && tmp->next->next && o_p(tmp->type) == 2
			&& o_p(tmp->next->type) == 0 && o_p(tmp->next->next->type) >= 3)
			return ((int)tmp->next->next->value[0]);
		tmp = tmp->next;
	}
	return (0);
}

int	syntax_error(char *line, t_token *token, t_env *env)
{
	int	x;

	if (precheck_line(line) != 0)
	{
		env->exit_s = 2;
		printf (RED"syntax error near unexpected token `newline'\n"RESET);
		return (1);
	}
	x = syntax_check(token);
	if (x != 0)
	{
		env->exit_s = 2;
		printf (RED"syntax error near unexpected token `%c'\n"RESET, (char)x);
		return (2);
	}
	return (0);
}
