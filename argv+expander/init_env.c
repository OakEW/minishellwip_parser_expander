/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:32:34 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/06 15:34:16 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

t_env	*init_env(char **envp)
{
	int		i;
	t_env	*env;

	i = 0;
	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->size = 0;
	env->exit_s = 0;
	while (envp[i++])
		env->cap = i * 2;
	env->env = malloc(sizeof(char *) * env->cap);
	if (!env->env)
		return (free_env(env), free(env), NULL);
	i = 0;
	while (envp[i])
	{
		env->env[i] = ft_strdup(envp[i]);
		if (!env->env[i])
			return (free_env(env), free(env), NULL);
		i++;
	}
	env->env[i] = NULL;
	env->size = i;
	return (env);
}

size_t	ft_strlen(const char *str)
{
	int	x;

	x = 0;
	if (!str)
		return (0);
	while (str[x] != '\0')
		x++;
	return (x);
}

char	*ft_strdup(char *s)
{
	char	*dup;
	int		i;

	i = 0;
	dup = (char *) malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (dup == 0)
		return (0);
	while (s[i] != 0)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
