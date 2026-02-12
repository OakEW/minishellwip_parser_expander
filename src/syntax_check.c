/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:33:02 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/11 13:40:00 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	precheck_line(char *line)
{
	int	parent;
	int	single_q;
	int	double_q;

	parent = 0;
	single_q = 0;
	double_q = 0;
	while (*line)
	{
		if (*line == '\'' && double_q == 0)
			single_q = !single_q;
		else if (*line == '\"' && single_q == 0)
			double_q = !double_q;
		else if (*line == '(' && !single_q && !double_q)
			parent++;
		else if (*line == ')' && !single_q && !double_q)
		{
			if (parent-- <= 0)
				return (258);
		}
		line++;
	}
	if (parent != 0 || single_q || double_q)
		return (258);
	return (0);
}

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

char	*syntax_check(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	if (o_p(token->type) == 1)
		return (tmp->value);
	while (tmp)
	{
		if (tmp->next == NULL && o_p(tmp->type) > 0 && o_p(tmp->type) < 3)
			return ("newline");
		if (o_p(tmp->type) == 1 && tmp->next && o_p(tmp->next->type) == 1)
			return (tmp->next->value);
		if (o_p(tmp->type) == 1 && tmp->next && o_p(tmp->next->type) == 4)
			return (tmp->next->value);
		if (o_p(tmp->type) == 2 && tmp->next && o_p(tmp->next->type) > 0)
			return (tmp->next->value);
		if (o_p(tmp->type) == 3 && tmp->next && o_p(tmp->next->type) == 1)
			return (tmp->next->value);
		if (o_p(tmp->type) == 3 && tmp->next && o_p(tmp->next->type) == 4)
			return (tmp->next->value);
		if (o_p(tmp->type) == 4 && tmp->next && (o_p(tmp->next->type) == 3 || o_p(tmp->next->type) == 0))
			return (tmp->next->value);
		if (tmp->next && tmp->next->next && o_p(tmp->type) == 2
			&& o_p(tmp->next->type) == 0 && o_p(tmp->next->next->type) >= 3)
			return (tmp->next->next->value);
		tmp = tmp->next;
	}
	return (NULL);
}

int	syntax_error(char *line, t_token *token, t_env *env)
{
	char	*x;

	x = syntax_check(token);
	if (x != NULL)
	{
		env->exit_s = 2;
		write (2, "syntax error near unexpected token `", 36);
		write (2, x, ft_strlen(x));
		write (2, "'\n", 2);
		return (2);
	}
	if (precheck_line(line) != 0)
	{
		env->exit_s = 2;
		write (2, "syntax error near unexpected token `newline'\n", 45);
		return (1);
	}
	return (0);
}
