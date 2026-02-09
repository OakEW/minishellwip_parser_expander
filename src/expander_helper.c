/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:32:16 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/09 14:55:00 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	check_q(char c, int *q_s, int *q_d)
{
	if (c == '\"' && *q_s == 0)
		*q_d = !(*q_d);
	if (c == '\'' && *q_d == 0)
		*q_s = !(*q_s);
	if (c == '$' && *q_s == 0)
		return (1);
	else
		return (0);
}

void	int_init(int *i)
{
	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
}

int	rm_char(char **str, int pos)
{
	int		i;

	i = pos;
	while ((*str)[i])
	{
		(*str)[i] = (*str)[i + 1];
		i++;
	}
	return (1);
}
