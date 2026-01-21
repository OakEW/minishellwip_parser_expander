/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 11:32:25 by ywang2            #+#    #+#             */
/*   Updated: 2026/01/21 12:31:57 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*make_token(char *str, t_token_type type)
{
	t_token	*token;
	int		len;

	len = 1;
	if (type > 7)
		len = 2;
	else if (type <= 2)
		len = token_len(str, type);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (str)
	{
		token->value = token_strndup(str, len, type);
		if (!token->value)
			return (free(token), NULL);
	}
	token->next = NULL;
	return (token);
}		

int	link_token(t_token **head, t_token **current, t_token_type type, char *str)
{
	t_token	*token;

	token = make_token(str, type);
	if (!token)
		return (-1);
	if (!*head)
		*head = token;
	else
		(*current)->next = token;
	*current = token;
	if (type >= 3 && type <= 7)
		return (1);
	else if (type > 7)
		return (2);
	return (token_len(str, type));
}

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
		if (*line == '(')
			parent++;
		if (*line == ')')
		{
			if (parent <= 0)
				return (-1);
			parent--;
		}
		single_q += (*line == '\'');
		double_q += (*line == '\"');
		line++;
	}
	if (parent != 0 || single_q % 2 != 0 || double_q % 2 != 0)
		return (-1);
	return (1);
}

t_token	*lexer(char	*line)
{
	int		i;
	int		n;
	t_token	*current;
	t_token	*head;
	int		type;

	if (precheck_line(line) < 0)
		return (write (2, "syntax error\n", 14), NULL);
	current = NULL;
	head = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && line[i] <= 32)
			i++;
		if (line[i] == 0)
			break ;
		type = is_operator(&line[i]);
		n = link_token(&head, &current, type, &line[i]);
		if (n < 0)
			return (free_tokens(head), NULL);
		i += n;
	}
	return (head);
}
