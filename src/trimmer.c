/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trimmer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:33:18 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/09 15:00:03 by ywang2           ###   ########.fr       */
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
		if (!curt->argv[i])
			return (-1);
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

int	expand_home(t_argv *curt, t_env *env)
{
	int		i;
	char	*home;
	char	*tmp;

	i = 0;
	home = find_var("HOME", env);
	while (curt->argv[i])
	{
		if (curt->argv[i][0] == '~' && curt->argv[i][1]== 0)
		{
			tmp = ft_strdup(home);
			if (!tmp)
				return (0);
			free (curt->argv[i]);
			curt->argv[i] = tmp;
		}
		i++;
	}
	return (1);
}

int	trim_expand(t_argv *curt, t_env *env)
{
	int	i;
	int	flag;

	i = 0;
	if (!check_wildcard(curt, env))
		return (0);
	if (!expand_home(curt, env))
		return (0);
	while (curt->argv[i])
	{
		if (curt->type <= 2)
		{
			flag = 10;
			if (!expander(&curt->argv[i], env))
				return (0);
			if (curt->argv[i][0] == 0)
			{
				flag = rm_empty(curt, i);
				if (flag == -1)
					return (malloc_fail(env), 0);
				if (flag == 1)
					break ;
				if (flag == 0)
					continue ;
			}
			trim_q(curt->argv[i]);
		}
		i++;
	}
	return (1);
}
