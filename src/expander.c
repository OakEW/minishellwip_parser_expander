/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:32:16 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/13 15:53:25 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	expand_home(t_argv *curt, t_env *env)
{
	int		i;
	char	*home;
	char	*tmp;

	i = 0;
	home = find_var("HOME", env);
	while (curt->argv[i])
	{
		if ((curt->argv[i][0] == '~' && curt->argv[i][1] == 0)
			|| (curt->argv[i][0] == '~' && curt->argv[i][1] == '/'))
		{
			tmp = ft_strjoin(home, &curt->argv[i][1]);
			if (!tmp)
				return (0);
			free (curt->argv[i]);
			curt->argv[i] = tmp;
		}
		i++;
	}
	return (1);
}

int	expander_helper(char **str, t_env *env)
{
	int		i[3];
	char	*var;

	int_init(i);
	while ((*str)[i[0]])
	{
		if (check_q((*str)[i[0]], &i[1], &i[2]))
		{
			i[0]++;
			if ((*str)[i[0]] == 0)
				break ;
			if ((*str)[i[0]] == '?')
				var = NULL;
			else if (!is_al((*str)[i[0]]))
				continue ;
			else
				var = find_var(&(*str)[i[0]], env);
			if (!replace_var(str, var, i[0], env))
				return (0);
			i[0] -= 2;
		}
		i[0]++;
	}
	return (1);
}

int	expander(t_argv *curt, t_env *env)
{
	int	i;

	i = 0;
	while (i < curt->argc)
	{
		if (!expander_helper(&(curt->argv[i]), env))
			return (0);
		i++;
	}
	return (1);
}

int	expand_all(t_argv *curt, t_env *env)
{
	if (curt->type > 2)
		return (0);
	if (!wildcards(curt, env))
		return (1);
	if (!expand_home(curt, env))
		return (2);
	rm_char(curt);
	if (!expander(curt, env))
		return (3);
	if (!trim_empty(curt))
		return (4);
	trim_quote(curt);
	return (0);
}
