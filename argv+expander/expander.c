#include "argv_env.h"

char	*find_var(char *str, t_env* env)
{
	int	i;
	int	j;

	j = 0;
	if (!str || !env || !env->env)
		return (NULL);
	while (env->env[j])
	{
		i = 0;
		while (env->env[j][i] && env->env[j][i] != '=' && str[i] && env->env[j][i] == str[i])
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
	int	 i;
	int	 j;
	int	 k;

	join = malloc(sizeof(char) * (len + 1));
	if (!join)
		return (NULL);
	i = 0;
	while (i < pos - 1)
	{
		join[i] = (*str)[i];
		i++;
	}
	j = 0;
	while (add && add[j])
		join[i++] = add[j++];
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

int	replace_var(char **str, char *add, int i, t_env *env)
{
	char	*exit_s;
	char	*new;
	int		len;
	int		subtract;

	len = ft_strlen(*str);
	if ((*str)[i] == '?')
	{
		exit_s = ft_itoa(env->exit_s);
		if (!exit_s)
			return (0);
		len = len - 2 + get_len(env->exit_s) + 1;
		new = var_join(str, exit_s, i, len);
		free(exit_s);
	}
	else
	{
		subtract = 0;
		while ((*str)[i + subtract] && is_al((*str)[i + subtract]))
			subtract++;
		len = len - subtract - 2 + ft_strlen(add) + 1;
		new = var_join(str, add, i, len);
	}
	if (!new)
		return (0);
	free(*str);
	*str = new;
	return (1);
}

int	expander(char **str, t_env* env)
{
	int		i;
	int		q_s;
	int		q_d;
	char	*var;

	i = 0;
	q_s = 0;
	q_d = 0;
	while((*str)[i])
	{
		if ((*str)[i] == '\"' && q_s == 0)
			q_d = !q_d;
		if ((*str)[i] == '\'' && q_d == 0)
			q_s = !q_s;
		if ((*str)[i] == '$' && q_s == 0)
		{
			i++;
			if ((*str)[i] == 0)
				break ;
			if ((*str)[i] == '?')
				var = NULL;
			else if (!is_al((*str)[i]))
				continue ;
			else
				var = find_var(&(*str)[i], env);
			if (!replace_var(str, var, i, env))
				return (0);
			i--;
		}
		i++;
	}
	return (1);
}
