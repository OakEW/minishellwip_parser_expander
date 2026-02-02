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
	else if (c == '=')
		return (2);
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

	i = 0;
	j = 0;
	if (is_al(str[i]) != 1)
		return (NULL);
	while (env->env[j])
	{
		while (is_al(str[i]) == 1 && str[i] == env->env[j][i])
			i++;
		if (env->env[j][i] == '=')
			return (&(env->env[j][i + 1]));
		i = 0;
		j++;
	}
	return (NULL);
}

void	free_exit_s(char **str, char *add, int x)
{
	if (str[0][x] == '?')
		free(add);
}

char *var_join(char **str, char *add, int x, int len)
{
	char	*join;
	int		i;
	int		j;

	i = 0;
	j = 0;
	join = malloc(sizeof(char) * (len + 1));
	if (!join)
		return (free_exit_s(str, add, x), NULL);
	while (i < x - 1)
	{
		join[i] = str[0][i];
		i++;
	}
	while (add[j])
	{
		join[i + j] = add[j];
		i++;
	}
	j = x;
	while (is_al(str[0][j]) || str[0][j] == '?')
		j++;
	while (str[0][j])
	{
		join[i] = str[0][j];
		j++;
		i++;
	}
	join[i] = 0;
	free_exit_s(str, add, x);
	free (*str);
	return (join);
}

int	replace_var(char **str, char *var, int i)
{
	char	*exit_s;
	int		len;

	len = ft_strlen(str[0]);
	exit_s = ft_itoa(exit_status);

	if (str[0][i] == '?')
	{
		len += get_len(exit_status);
		*str = var_join(str, exit_s, i, len);
	}
	else
	{
		len += ft_strlen(var);
		*str = var_join(str, var, i, len);
	}
	if (!*str)
		return (0);
	// printf ("env ->%s\n", *str);
	return (1);
}

// int	expander(char **str, t_env* env)
// {
// 	int		i;
// 	int		q_s;
// 	int		q_d;
// 	char	*var;

// 	i = 0;
// 	q_s = 0;
// 	q_d = 0;
// 	while(str[0][i])
// 	{
// 		if (str[0][i] == '\"' && q_s == 0)
// 			q_d = !q_d;
// 		if (str[0][i] == '\'' && q_d == 0)
// 			q_s = !q_s;
// 		if (str[0][i] == '$' && q_s == 0)
// 		{
// 			if (str[0][++i] == 0)
// 				return (0);
// 			var = find_var(&str[0][i], env);
// 			printf ("env ->%s\n", var);
// 			if (var || str[0][i] == '?')
// 			{
// 				// replace_var(str, var, i);
// 				if (!*str)
// 					return (0);
// 			}
// 		}
// 		i++;
// 	}
// 	return (0);
// }

int	expander(char **str, t_env* env)
{
int i;
int q_s;
int q_d;

i = 0;
q_s = 0;
q_d = 0;
while (str[0][i])
{
	if (str[0][i] == '\'' && !q_d)
	{
		q_s = !q_s;
		i++;
	}
	else if (str[0][i] == '\"' && !q_s)
	{
		q_d = !q_d;
		i++;
	}
	else if (str[0][i] == '$' && !q_s)
	{
		i++;
		if (str[0][i] == 0)
			break;
		char *var = find_var(&str[0][i], env);
		printf("env -> %s\n", var);
		// replace_var(str, var, i);
	}
	else
		i++;
}
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
