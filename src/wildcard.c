/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:53:44 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/09 15:00:08 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

char	**join_wild_helper(t_argv *curt, char **entry)
{
	char	**new;
	int		len;

	len = 0;
	while (entry[len])
		len++;
	new = malloc(sizeof(char *) * (curt->argc + len));
	if (!new)
		return (NULL);
	curt->argc = curt->argc + len - 1;
	return (new);
}

int	join_wild(t_argv *curt, int pos, char **entry, t_env *env)
{
	char	**new;
	int		x;
	int		i;
	int		j;

	i = 0;
	x = 0;
	j = 0;
	new = join_wild_helper(curt, entry);
	if (!new)
		return (malloc_fail(env), 0);
	while (i < pos)
	{
		new[i] = ft_strdup(curt->argv[x]);
		if (!new[i])
			return (malloc_fail(env), free_strstr(new), 0);
		x++;
		i++;
	}
	x++;
	while (entry[j])
	{
		new[i] = ft_strdup(entry[j]);
		if (!new[i])
			return (malloc_fail(env), free_strstr(new), 0);
		i++;
		j++;
	}
	while (curt->argv[x])
	{
		new[i] = ft_strdup(curt->argv[x]);
		if (!new[i])
			return (malloc_fail(env), free_strstr(new), 0);
		i++;
		x++;
	}
	new[i] = 0;
	free_strstr(curt->argv);
	curt->argv = new;
	return (1);
}

int	wildcards(t_argv *curt, t_env *env)
{
	int		i;
	int		len;
	char	**entry;

	i = 0;
	len = 0;
	entry = get_entry(env);
	if (!entry)
		return (0);
	sort_entry(entry);
	while (curt->argv[i])
	{
		if (curt->argv[i][0] == '*' && curt->argv[i][1] == 0)
		{		
			if (!join_wild(curt, i, entry, env))
				return (free_strstr(entry), 0);
			i = 0;
		}
		i++;
	}
	if (entry)
		free_strstr(entry);
	return (1);
}

int	check_wildcard(t_argv *curt, t_env *env)
{
	int	i;

	i = 0;
	while (curt->argv[i])
	{
		if (curt->argv[i][0] == '*' && curt->argv[i][1] == 0)
		{
			if (!wildcards(curt, env))
				return (0);
		}
		i++;
	}
	return (1);
}
