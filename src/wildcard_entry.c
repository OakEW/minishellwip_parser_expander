/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_entry.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:53:44 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/10 12:30:26 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	entry_len(t_env *env)
{
	DIR				*cwd;
	struct dirent	*entry;
	int				i;

	i = 0;
	cwd = opendir(".");
	if (cwd == NULL)
		return (perror("opendir"), env->exit_s = errno, -1);
	entry = readdir(cwd);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.')
			i++;
		entry = readdir(cwd);
	}
	closedir(cwd);
	return (i);
}

char	**get_entry(t_env *env)
{
	DIR				*cwd;
	struct dirent	*entry;
	char			**stash;
	int				i;

	i = entry_len(env);
	if (i < 0)
		return (NULL);
	stash = malloc(sizeof(char *) * (i + 1));
	if (!stash)
		return (NULL);
	cwd = opendir(".");
	if (cwd == NULL)
		return (perror("opendir"), free_strstr(stash), env->exit_s = errno, NULL);
	i = 0;
	entry = readdir(cwd);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.')
		{
			stash[i] = ft_strdup(entry->d_name);
			if (!stash[i])
				return (free_strstr(stash), closedir(cwd), NULL);
			i++;
		}
		entry = readdir(cwd);
	}
	stash[i] = NULL;
	closedir(cwd);
	return (stash);
}
