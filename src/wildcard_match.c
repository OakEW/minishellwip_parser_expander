/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_match.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:53:44 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/20 13:00:09 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	match(char *pattern, char *flag, char *str)
{
	if (!*pattern && !*str)
		return (1);
	if (*pattern == '*' && *flag == 'w')
	{
		while (*(pattern + 1) == '*' && *(flag + 1) == 'w')
			pattern++;
		if (match(pattern + 1, flag + 1, str))
			return (1);
		if (*str && match(pattern, flag, str + 1))
			return (1);
		return (0);
	}
	if (*pattern == *str)
		return (match(pattern + 1, flag + 1, str + 1));
	return (0);
}

int	pattern_matching(t_wild *wild, t_entry *entry)
{
	int	n;

	n = 0;
	while (entry->entry[n])
	{
		if (wild->str[0] != '.' && entry->entry[n][0] == '.')
		{
			free (entry->entry[n]);
			entry->entry[n] = NULL;
		}
		else
		{
			if (match(wild->str, wild->flag, entry->entry[n]))
				entry->match++;
			else
			{
				free (entry->entry[n]);
				entry->entry[n] = NULL;
			}
		}
		n++;
	}
	return (0);
}
