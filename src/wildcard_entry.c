/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_entry.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:53:44 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/12 11:10:26 by ywang2           ###   ########.fr       */
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

int	get_entry(t_env *env, t_entry *stash)
{
	DIR				*cwd;
	struct dirent	*entry;
	int				i;

	i = entry_len(env);
	if (i < 0)
		return (0);
	stash->cap = i;
	stash->match = 0;
	stash->entry = malloc(sizeof(char *) * (i + 1));
	if (!stash->entry)
		return (0);
	cwd = opendir(".");
	if (cwd == 0)
		return (perror("opendir"), free_strstr(stash->entry), env->exit_s = errno, 0);
	i = 0;
	entry = readdir(cwd);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.')
		{
			stash->entry[i] = ft_strdup(entry->d_name);
			if (!stash->entry[i])
				return (free_strstr(stash->entry), closedir(cwd), 0);
			i++;
		}
		entry = readdir(cwd);
	}
	stash->entry[i] = NULL;
	sort_entry(stash->entry);
	closedir(cwd);
	return (1);
}
