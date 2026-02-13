/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trimmer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:33:18 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/13 16:00:47 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

void	trim_q(char **s)
{
	int	i;
	int	n;
	int	sq;
	int	dq;

	i = 0;
	n = 0;
	sq = 0;
	dq = 0;
	while ((*s)[i])
	{
		if ((*s)[i] == '\'' && !dq)
			sq = !sq;
		else if ((*s)[i] == '\"' && !sq)
			dq = !dq;
		else
			(*s)[n++] = (*s)[i];
		i++;
	}
	(*s)[n] = 0;
}

void	trim_quote(t_argv *curt)
{
	int	i;

	i = 0;
	while (i < curt->argc)
	{
		trim_q(&(curt->argv[i]));
		i++;
	}
}

int	rm_empty(t_argv *curt, int i)
{
	char	*tmp;

	if (curt->argc == 1)
	{
		free (curt->argv[0]);
		curt->argv[0] = NULL;
		curt->argc = 0;
		return (-1);
	}
	while (i < curt->argc - 1)
	{
		tmp = ft_strdup(curt->argv[i + 1]);
		if (!tmp)
			return (0);
		free (curt->argv[i]);
		curt->argv[i] = tmp;
		i++;
	}
	free (curt->argv[i]);
	curt->argv[i] = NULL;
	curt->argc--;
	return (1);
}

int	trim_empty(t_argv *curt)
{
	int	i;
	int	flag;

	i = 0;
	while (curt->argv[i])
	{
		flag = 10;
		if (curt->argv[i][0] == 0)
		{
			flag = rm_empty(curt, i);
			if (flag == 0)
				return (0);
			if (flag == -1)
				break ;
			if (flag == 1)
				continue ;
		}
		i++;
	}
	return (1);
}
