/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:53:44 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/20 16:02:53 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	join_wild(t_argv *curt, int pos, t_entry *entry)
{
	char	**new;
	int		i;

	i = 0;
	if (entry->match == 0)
		return (2);
	new = malloc(sizeof(char *) * (curt->argc + entry->match));
	if (!new)
		return (0);
	i = mutistrdup(new, curt->argv, pos, 0);
	if (i < 0)
		return (free_strstr(new), 0);
	i = mutistrdup(new, entry->entry, entry->cap, i);
	if (i < 0)
		return (free_strstr(new), 0);
	i = mutistrdup(new, &(curt->argv[pos + 1]), curt->argc - pos - 1, i);
	if (i < 0)
		return (free_strstr(new), 0);
	new[i] = 0;
	free_strstr(curt->argv);
	curt->argv = new;
	curt->argc = i;
	return (1);
}

int	init_wild(t_wild *wild, int *i)
{
	int	n;

	n = 0;
	while (wild->str[i[0]])
	{
		if (wild->str[i[0]] == '\'' && !i[2])
			i[1] = !i[1];
		else if (wild->str[i[0]] == '\"' && !i[1])
			i[2] = !i[2];
		else
		{
			wild->str[n] = wild->str[i[0]];
			wild->flag[n] = 'n';
			if (wild->str[n] == '*' && !i[1] && !i[2])
			{
				wild->flag[n] = 'w';
				i[4] = 1;
			}
			n++;
		}
		(i[0])++;
	}
	wild->str[n] = 0;
	wild->flag[n] = 'n';
	return (i[4]);
}

int	wild_catcher(char *str, t_wild *wild)
{
	int	i[4];

	int_init(i, 4);
	wild->str = ft_strdup(str);
	if (!wild->str)
		return (-1);
	wild->flag = ft_strdup(str);
	if (!wild->flag)
		return (-1);
	return (init_wild(wild, i));
}

int	wildcards(t_argv *curt, t_env *env)
{
	int		i;
	t_entry	entry;
	t_wild	wild;

	i = 0;
	while (curt->argv[i])
	{
		if (wild_catcher(curt->argv[i], &wild) > 0)
		{
			if (!get_entry(env, &entry))
				return (0);
			pattern_matching(&wild, &entry);
			if (!join_wild(curt, i, &entry))
				return (free_entry(&entry), 0);
			if (entry.match)
				i = i + entry.match - 1;
			free_entry(&entry);
		}
		if (wild.str)
			free(wild.str);
		if (wild.flag)
			free(wild.flag);
		i++;
	}
	return (1);
}
