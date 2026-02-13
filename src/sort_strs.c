/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_strs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:53:44 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/13 15:54:41 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	entry_strcmp(char *s1, char *s2)
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
			if (entry_strcmp(entry[i], entry [j]) > 0)
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

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*tmp_d;
	unsigned char	*tmp_s;

	if (dest == NULL && src == NULL)
		return (NULL);
	tmp_d = (unsigned char *)dest;
	tmp_s = (unsigned char *)src;
	while (n > 0)
	{
		*tmp_d = *tmp_s;
		tmp_d++;
		tmp_s++;
		n--;
	}
	return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;

	join = (char *) malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (join == 0)
		return (NULL);
	ft_memcpy(join, s1, ft_strlen(s1));
	ft_memcpy(&join[ft_strlen(s1)], s2, ft_strlen(s2));
	join[ft_strlen(s1) + ft_strlen(s2)] = 0;
	return (join);
}
