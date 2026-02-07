/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_itoa.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:32:27 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/06 15:32:28 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	is_al(char c)
{
	if (c == '_')
		return (1);
	else if (c >= 'a' && c <= 'z')
		return (1);
	else if (c >= 'A' && c <= 'Z')
		return (1);
	else if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	get_len(int n)
{
	int		i;
	long	nb;

	i = 1;
	nb = (long)n;
	if (nb < 0)
	{
		nb *= -1;
		i = 2;
	}
	while (nb > 9)
	{
		nb = nb / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		i;
	long	m;

	i = get_len(n);
	m = (long)n;
	if (m < 0)
		m *= -1;
	str = (char *) malloc(sizeof(char) * (i + 1));
	if (str == NULL)
		return (NULL);
	str[0] = '-';
	str[i] = '\0';
	i--;
	while (m > 9)
	{
		str[i] = m % 10 + '0';
		m = m / 10;
		i--;
	}
	str[i] = m + '0';
	return (&str[0]);
}
