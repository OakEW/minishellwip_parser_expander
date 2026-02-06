/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trimmer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:33:18 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/06 15:44:05 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	rm_empty(t_argv *curt, int i)
{
	if (curt->argc == 1)
	{
		free (curt->argv[0]);
		curt->argv[0] = NULL;
		curt->argc = 0;
		return (1);
	}
	while (i < curt->argc - 1)
	{
		free (curt->argv[i]);
		curt->argv[i] = ft_strdup(curt->argv[i + 1]);
		i++;
	}
	free (curt->argv[i]);
	curt->argv[i] = NULL;
	curt->argc--;
	return (0);
}

void	trim_q(char *s)
{
	int	i;
	int	n;
	int	sq;
	int	dq;

	i = 0;
	n = 0;
	sq = 0;
	dq = 0;
	while (s[i])
	{
		if (s[i] == '\'' && !dq)
			sq = !sq;
		else if (s[i] == '\"' && !sq)
			dq = !dq;
		else
			s[n++] = s[i];
		i++;
	}
	s[n] = 0;
}

int	trim_expand(t_argv *curt, t_env *env)
{
	int	i;
	int	flag;

	i = 0;
	wildcards(curt, env);
	while (curt->argv[i])
	{
		if (curt->type <= 2)
		{
			flag = 10;
			expander(&curt->argv[i], env);
			if (curt->argv[i][0] == 0)
			{
				flag = rm_empty(curt, i);
				if (flag == 1)
					break ;
				if (flag == 0)
					continue ;
			}
			trim_q(curt->argv[i]);
		}
		i++;
	}
	return (0);
}
