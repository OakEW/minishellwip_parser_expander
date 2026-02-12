/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:53:44 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/12 13:01:38 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	join_wild(t_argv *curt, int pos, t_entry *entry)
{
	char	**new;
	int		x;
	int		i;
	int		j;

	i = 0;
	x = 0;
	if (entry->match == 0)					// if no match, do nothing
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

int	pattern_matching(char *pat, t_entry *entry) //matching pattern str (curt->argv[i]) with strstrs entry->entry
{
	entry->match = entry->cap - 1;
	free (entry->entry[2]);
	entry->entry[2] = NULL;
	// int	n;
	// int	m;
	// int	j;

	// n = 0;
	// j = 0;
	// while (entry->entry[n])
	// {
	// 	m = 0;
	// 	while (pat[j] && entry->entry[n][m])
	// 	{
	// 		while (pat[j] != '*' && pat[j] == entry->entry[n][m])
	// 		{
	// 			j++;
	// 			m++;
	// 		}
	// 		if (pat[j] != '*')
	// 			break ;
	// 		while (pat[j])
	// 			j++;
	// 		while (entry->entry[n][m])
	// 			m++;
	// 		j--;
	// 		m--;
	// 		while (pat[j] != '*' && pat[j] == entry->entry[n][m])
	// 		{
	// 			j--;
	// 			m--;
	// 		}
	// 	}
	// 	if (pat[j] != '*')
	// 	{
	// 		free (entry->entry[n]);
	// 		entry->entry[n] = NULL;
	// 	}
	// 	else
	// 		entry->match++;
	// 	j = 0;
	// 	n++;
	// }
	return (0);
}

int	check_wildcard(t_argv *curt, t_env *env)
{
	int		i;
	int		j;
	t_entry entry;

	i = 0;
	while (curt->argv[i])
	{
		j = 0;
		while (curt->argv[i][j] && curt->argv[i][j] != '*')
			j++;
		if (curt->argv[i][j] == '*')				//find a string(curt->argv[i]) with * in it 
		{
			if (!get_entry(env, &entry))			//get entry, sort entry
				return (0);
			pattern_matching(curt->argv[i], &entry);
			if (!join_wild(curt, i, &entry))		//do wildcard on this string(curt->argv[i])
				return (free_entry(&entry), 0);
			if (entry.match == 0)
				entry.match = 1;
			i = i + entry.match - 1;
			free_entry(&entry);
		}
		i++;
	}
	return (1);
}
