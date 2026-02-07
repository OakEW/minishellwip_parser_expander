/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:53:44 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/07 16:10:25 by ywang2           ###   ########.fr       */
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
		return (perror("opendir"), env->exit_s = errno, 0);
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
	if (!i)
		return (NULL);
	stash = malloc(sizeof(char *) * (i + 1));
	if (!stash)
		return (perror("malloc"), env->exit_s = ENOMEM, NULL);
	cwd = opendir(".");
	if (cwd == NULL)
		return (perror("opendir"), env->exit_s = errno, NULL);
	i = 0;
	entry = readdir(cwd);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.')
			stash[i++] = ft_strdup(entry->d_name);
		entry = readdir(cwd);
	}
	stash[i] = NULL;
	closedir(cwd);
	return (stash);
}

char	**join_wild_helper(t_argv *curt, char **entry, t_env *env)
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

	i = 0;
	x = 0;
	new = join_wild_helper(curt, entry, env);
	if (!new)
		return (perror("malloc"), env->exit_s = ENOMEM, 0);
	while (i < pos)
		new[i++] = ft_strdup(curt->argv[x++]);
	x++;
	while (*entry)
		new[i++] = ft_strdup(*entry++);
	while (curt->argv[x])
		new[i++] = ft_strdup(curt->argv[x++]);
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
