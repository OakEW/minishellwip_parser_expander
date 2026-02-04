#include "lexer.h"

extern int	exit_status;


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
	if (env)
		free (env);
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

int	replace_var(char **str, char *add, int i)
{
	char	*exit_s;
	char	*new;
	int		len;
	int		subtract;

	len = ft_strlen(*str);
	if ((*str)[i] == '?')
	{
		exit_s = ft_itoa(exit_status);
		if (!exit_s)
			return (0);
		len = len - 2 + get_len(exit_status) + 1;
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
			if (!replace_var(str, var, i))
				return (0);
			i--;
		}
		i++;
		// if ((*str)[i] == '$' && q_s != 0)
		// 	i++;
		// if ((*str)[i] != '$')
		// 	i++;
	}
	return (1);
}

// void	rm_node(t_argv *head, t_argv *rm)
// {
// 	t_argv	*tmp;
// 	int		i;

// 	i = 0;
// 	tmp = head;
// 	while(tmp && tmp->next && tmp->next != rm)
// 		tmp = tmp->next;
// 	tmp->next = rm->next;
// 	while (i < rm->argc)
// 	{
// 		free(rm->argv[i]);
// 		i++;
// 	}
// 	free(rm->argv);
// 	free(rm);
// }

int	rm_empty(t_argv *curt, int i)
{
	// if (curt->argc == 1 && !head->next) // if only 1 node && t_argv *current is emt, == !line
	// 	return (-1);
	if (curt->argc == 1)	// if t_argv *current is emt, set node to NULL
	{
		free (curt->argv[0]);
		curt->argv[0] = NULL;
		curt->argc = 0;
		return (1);
	}
	while (i < curt->argc - 1)	// else remove argv[i] from node
	{
		free (curt->argv[i]);
		curt->argv[i] = ft_strdup(curt->argv[i + 1]);
		i++;
	}
	free (curt->argv[i]);
	curt->argv[i] = NULL;
	curt->argc--;
	return (0);
}

void trim_q(char *s)
{
	int	i;
	int	n;
	int	sq;
	int	dq;

	i = 0;
	n = 0;
	sq = 0;
	dq = 0;
	while (s[i])
	{
		if (s[i] == '\'' && !dq)
			sq = !sq;
		else if (s[i] == '\"' && !sq)
			dq = !dq;
		else
			s[n++] = s[i];
		i++;
	}
	s[n] = 0;
}

int	trim_expand(t_argv *curt, t_env *env)
{
	int	i;
	int	flag;

	i = 0;
	while (curt->argv[i])
	{
		if (curt->type <= 2)
		{
			flag = 10;
			expander(&curt->argv[i], env);
			if (curt->argv[i][0] == 0)
			{
				flag = rm_empty(curt, i);
				if (flag == -1) //return -1 if !line after expand
					return (-1);
				if (flag == 1)
					break;
				if (flag == 0)
					continue;
			}
			trim_q(curt->argv[i]);
		}
		i++;
	}
	return (0);
}
