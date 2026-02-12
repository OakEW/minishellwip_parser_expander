/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rm_char.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:32:16 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/12 15:36:42 by ywang2           ###   ########.fr       */
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

void	rm_char_helper(char **str)
{
	int		i[3];
	int		pos;

	int_init(i);
	while ((*str)[i[0]])
	{
		check_q((*str)[i[0]], &i[1], &i[2]);
		if ((*str)[i[0]] == '$')
		{
			if (!i[1] && !i[2] && (*str)[i[0] + 1] && ((*str)[i[0] + 1] == '\'' || (*str)[i[0] + 1] == '\"'))
			{
				pos = i[0];
				while ((*str)[pos])
				{
					(*str)[pos] = (*str)[pos + 1];
					pos++;
				}
				continue;
			}
		}
		i[0]++;
	}
}

void	rm_char(t_argv *curt)
{
	int	i;

	i = 0;
	while (i < curt->argc)
	{
		rm_char_helper(&(curt->argv[i]));
		i++;
	}
}
