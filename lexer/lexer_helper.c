/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 11:32:25 by ywang2            #+#    #+#             */
/*   Updated: 2026/01/21 16:57:44 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	is_operator(char *str)
{
	if (*str == '|' && *(str + 1) != '|')
		return (PIPE);
	else if (*str == '(')
		return (PARENT_O);
	else if (*str == ')')
		return (PARENT_C);
	else if (*str == '<' && *(str + 1) != '<')
		return (INPUT);
	else if (*str == '>' && *(str + 1) != '>')
		return (OUTPUT);
	else if (*str == '&' && *(str + 1) == '&')
		return (THEN);
	else if (*str == '|' && *(str + 1) == '|')
		return (ELSE);
	else if (*str == '<' && *(str + 1) == '<')
		return (HEREDOC);
	else if (*str == '>' && *(str + 1) == '>')
		return (APPEND);
	else if (*str == '\'')
		return (SINGLE_Q);
	else if (*str == '\"')
		return (DOUBLE_Q);
	return (0);
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

int	token_len(char *str, t_token_type type)
{
	int		x;
	char	q;

	x = 0;
	if (!str)
		return (0);
	while (str[x])
	{
		if (str[x] == '\'' || str[x] == '\"')
		{
			q = str[x++];
			while (str[x] && str[x] != q)
				x++;
			if (str[x] == q)
				x++;
		}
		else if (str[x] <= 32 || is_operator(&str[x]))
			break ;
		else if (str[x] > 32 && !is_operator(&str[x]))
			x++;
	}
	return (x);
}

char	*token_strndup(const char *s, int n)
{
	char	*dup;
	int		i;

	if (n <= 0)
		return (NULL);
	i = 0;
	dup = (char *) malloc(sizeof(char) * (n + 1));
	if (dup == 0)
		return (0);
	while (i < n)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
