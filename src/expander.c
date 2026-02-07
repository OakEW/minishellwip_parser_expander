/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:32:16 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/07 17:27:24 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

char	*find_var(char *str, t_env *env)
{
	int	i;
	int	j;

	j = 0;
	if (!str || !env || !env->env)
		return (NULL);
	while (env->env[j])
	{
		i = 0;
		while (env->env[j][i] && env->env[j][i] != '='
			&& str[i] && env->env[j][i] == str[i])
			i++;
		if (env->env[j][i] == '=' && !is_al(str[i]))
			return (&(env->env[j][i + 1]));
		j++;
	}
	return (NULL);
}

char	*var_join(char **str, char *add, int pos, int len)
{
	char	*join;
	int		i;
	int		k;

	join = malloc(sizeof(char) * (len + 1));
	if (!join)
		return (NULL);
	i = 0;
	while (i < pos - 1)
	{
		join[i] = (*str)[i];
		i++;
	}
	while (add && *add)
		join[i++] = *add++;
	k = pos;
	while ((*str)[k] && is_al((*str)[k]))
		k++;
	if ((*str)[pos] == '?')
		k = pos + 1;
	while ((*str)[k])
		join[i++] = (*str)[k++];
	join[i] = '\0';
	return (join);
}

char	*replace_var_helper(char **str, int i, t_env *env)
{
	char	*exit_s;
	char	*new;
	int		len;

	exit_s = ft_itoa(env->exit_s);
	if (!exit_s)
		return (NULL);
	len = ft_strlen(*str) - 2 + get_len(env->exit_s) + 1;
	new = var_join(str, exit_s, i, len);
	if (!new)
		return (free(exit_s), NULL);
	free(exit_s);
	return (new);
}

int	replace_var(char **str, char *add, int i, t_env *env)
{
	char	*new;
	int		len;
	int		subtract;

	if ((*str)[i] == '?')
	{
		new = replace_var_helper(str, i, env);
		if (!new)
			return (0);
	}
	else
	{
		subtract = 0;
		while ((*str)[i + subtract] && is_al((*str)[i + subtract]))
			subtract++;
		len = ft_strlen(*str) - subtract - 2 + ft_strlen(add) + 1;
		new = var_join(str, add, i, len);
		if (!new)
			return (0);
	}
	free(*str);
	*str = new;
	return (1);
}

int	expander(char **str, t_env *env)
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
				return (malloc_fail(env), 0);
			i[0] -= 2;
		}
		i[0]++;
	}
	return (1);
}
