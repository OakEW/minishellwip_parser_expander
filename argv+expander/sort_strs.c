/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_strs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:53:44 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/07 16:16:03 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
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
