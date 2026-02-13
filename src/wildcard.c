/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:53:44 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/13 16:09:14 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

// int	joint_wild_helper()

int	join_wild(t_argv *curt, int pos, t_entry *entry)
{
	char	**new;
	int		x;
	int		i;
	int		j;

	i = 0;
	x = 0;
	if (entry->match == 0)
		return (2);
	new = malloc(sizeof(char *) * (curt->argc + entry->match));
	if (!new)
		return (0);
	while (i < pos)
	{
		new[i] = ft_strdup(curt->argv[x]);
		if (!new[i])
			return (free_strstr(new), 0);
		x++;
		i++;
	}
	x++;
	j = 0;
	while (j < entry->cap)
	{
		if (entry->entry[j] != NULL)
		{
			new[i] = ft_strdup(entry->entry[j]);
			if (!new[i])
				return (free_strstr(new), 0);
			i++;
		}
		j++;
	}
	while (curt->argv[x])
	{
		new[i] = ft_strdup(curt->argv[x]);
		if (!new[i])
			return (free_strstr(new), 0);
		i++;
		x++;
	}
	new[i] = 0;
	free_strstr(curt->argv);
	curt->argv = new;
	curt->argc = curt->argc + entry->match - 1;
	return (1);
}

int	match(char *pattern, char *str)
{
	if (!*pattern && !*str)
		return (1);
	if (*pattern == '*')
	{
		while (*(pattern + 1) == '*')
			pattern++;
		if (match(pattern + 1, str))
			return (1);
		if (*str && match(pattern, str + 1))
			return (1);
		return (0);
	}
	if (*pattern == *str)
		return (match(pattern + 1, str + 1));
	return (0);
}

int	pattern_matching(char *pattern, t_entry *entry)
{
	int	n;
	int	ret;

	n = 0;
	sort_entry(entry->entry);
	while (entry->entry[n])
	{
		ret = match(pattern, entry->entry[n]);
		if (ret)
			entry->match++;
		else
		{
			free (entry->entry[n]);
			entry->entry[n] = NULL;
		}
		n++;
	}
	return (0);
}

int	wildcards(t_argv *curt, t_env *env)
{
	int		i;
	int		j[3];
	t_entry	entry;

	i = 0;
	int_init(j);
	while (curt->argv[i])
	{
		j[0] = 0;
		while (curt->argv[i][j[0]] && curt->argv[i][j[0]] != '*')
			check_q(curt->argv[i][j[0]++], &j[1], &j[2]);
		if (curt->argv[i][j[0]] == '*' && !j[1] && !j[2])
		{
			if (!get_entry(env, &entry))
				return (0);
			pattern_matching(curt->argv[i], &entry);
			if (!join_wild(curt, i, &entry))
				return (free_entry(&entry), 0);
			if (entry.match)
				i = i + entry.match - 1;
			free_entry(&entry);
		}
		i++;
	}
	return (1);
}
