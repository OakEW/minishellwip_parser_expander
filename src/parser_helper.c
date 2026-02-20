/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:32:57 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/20 11:22:37 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		return (free(argv), -1);
	while (n < argv->argc)
	{
		argv->argv[n] = ft_strdup(token->value);
		if (!argv->argv[n])
			return (free_argv(argv), -1);
		token = token->next;
		n++;
	}
	argv->argv[n] = 0;
	return (0);
}
