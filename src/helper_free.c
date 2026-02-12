/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:53:44 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/12 12:56:21 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

void	free_strstr(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		free (s[i++]);
	free(s);
}

void	free_env(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->size)
	{
		if (env->env[i])
			free (env->env[i]);
		i++;
	}
	if (env->env)
		free (env->env);
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
			while (i < tmp->argc)
			{
				if (tmp->argv[i])
					free(tmp->argv[i]);
				i++;
			}
			free(tmp->argv);
		}
		free(tmp);
	}
}

void	free_entry(t_entry *entry)
{
	int	i;

	i = 0;
	while (i < entry->cap)
	{
		if (entry->entry[i])
			free (entry->entry[i]);
		i++;
	}
	free(entry->entry);
}
