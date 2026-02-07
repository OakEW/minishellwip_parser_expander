/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_strs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:53:44 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/07 18:01:25 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

float	ft_strcmp(char *s1, char *s2)
{
	int		i;
	float	x;
	float	y;

	i = 0;
	while (s1[i] && s2[i])
	{
		x = (float)s1[i];
		if (x >= 'A' && x<= 'Z')
			x += 31.5;
		y = (float)s2[i];
		if (y >= 'A' && y<= 'Z')
			y += 31.5;
		if (x != y)
			break;
		i++;
	}
	return (x - y);
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
