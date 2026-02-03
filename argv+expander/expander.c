#include "lexer.h"

extern int	exit_status;

int	is_al(char c)
{
	if (c == '_')
		return (1);
	else if (c >= 'a' && c <= 'z')
		return (1);
	else if (c >= 'A' && c <= 'Z')
		return (1);
	else if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	get_len(int n)
{
	int		i;
	long	nb;

	i = 1;
	nb = (long)n;
	if (nb < 0)
	{
		nb *= -1;
		i = 2;
	}
	while (nb > 9)
	{
		nb = nb / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		i;
	long	m;

	i = get_len(n);
	m = (long)n;
	if (m < 0)
		m *= -1;
	str = (char *) malloc(sizeof(char) * (i + 1));
	if (str == NULL)
		return (NULL);
	str[0] = '-';
	str[i] = '\0';
	i--;
	while (m > 9)
	{
		str[i] = m % 10 + '0';
		m = m / 10;
		i--;
	}
	str[i] = m + '0';
	return (&str[0]);
}

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

void	free_exit_s(char **str, char *add, int x)
{
	if (x == '?')
		free(add);
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

char	*replace_var_helper(char **str, char *var, int i, int len)
{
	char	*new;
	int	 var_len;

	var_len = ft_strlen(var);
	len--;
	while (is_al((*str)[i + len - ft_strlen(*str) + 1]))
		len--;
	len += var_len;
	new = var_join(str, var, i, len);
	if (!new)
		return (NULL);
	return (new);
}

int	replace_var(char **str, char *var, int i)
{
	char	*exit_s;
	char	*new;
	int	 len;

	len = ft_strlen(*str);
	if ((*str)[i] == '?')
	{
		exit_s = ft_itoa(exit_status);
		if (!exit_s)
			return (0);
		len = len - 1 + get_len(exit_status);
		new = var_join(str, exit_s, i, len);
		free(exit_s);
	}
	else
		new = replace_var_helper(str, var, i, len);
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
			var = find_var(&(*str)[i], env);
			if (!replace_var(str, var, i))
				return (0);
			i--;
		}
		i++;
	}
	return (0);
}


void	free_env(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->size)
	{
		free (env->env[i]);
		i++;
	}
	if (env->env)
		free (env->env);
}

t_env *init_env(char **envp)
{
	int	i;
	t_env	*env;

	i = 0;
	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->size = 0;
	while (envp[i])
		i++;
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
