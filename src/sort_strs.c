/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_strs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:53:44 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/11 12:43:22 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	ft_strcmp(char *s1, char *s2)
{
	int		i;
	char	x;
	char	y;
	char	lx;
	char	ly;

	i = 0;
	while (s1[i] && s2[i])
	{
		x = s1[i];
		y = s2[i];
		lx = x;
		ly = y;
		if (x >= 'A' && x <= 'Z')
			lx += 32;
		if (y >= 'A' && y <= 'Z')
			ly += 32;
		if (lx != ly)
			return (lx - ly);
		if (x != y)
			return (x - y);
		i++;
	}
	return (s1[i] - s2[i]);
}

void	sort_entry(char **entry)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (entry[i])
	{
		j = i + 1;
		while (entry[i] && entry[j])
		{
			if (ft_strcmp(entry[i], entry [j]) > 0)
			{
				tmp = entry[i];
				entry[i] = entry[j];
				entry[j] = tmp;
			}
			j++;
		}
		i++;
	}
}