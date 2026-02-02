/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 22:10:00 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/02 22:10:00 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

extern int  g_exit_status;

static int  is_al(char c)
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

static int  get_len(int n)
{
    int     i;
    long    nb;

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

static char *ft_itoa(int n)
{
    char    *str;
    int     i;
    long    m;

    i = get_len(n);
    m = (long)n;
    if (m < 0)
        m *= -1;
    str = (char *)malloc(sizeof(char) * (i + 1));
    if (!str)
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
    return (str);
}

static char *find_var(char *str, t_env *env)
{
    int i;
    int j;

    j = 0;
    if (!str || !env || !env->env)
        return (NULL);
    while (env->env[j])
    {
        i = 0;
        while (env->env[j][i] && env->env[j][i] != '=' && str[i]
            && env->env[j][i] == str[i])
            i++;
        if (env->env[j][i] == '=' && !is_al(str[i]))
            return (&env->env[j][i + 1]);
        j++;
    }
    return (NULL);
}

static void free_exit_s(char **str, char *add, int is_q)
{
    if (is_q)
        free(add);
    (void)str;
}

static char *var_join(char **str, char *add, int pos, int len)
{
    char    *join;
    int     i;
    int     j;
    int     k;

    join = (char *)malloc(sizeof(char) * (len + 1));
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

static int  replace_var(char **str, char *var, int i)
{
    char    *exit_s;
    char    *new;
    int     len;
    int     var_len;

    len = (int)ft_strlen(*str);
    if ((*str)[i] == '?')
    {
        exit_s = ft_itoa(g_exit_status);
        if (!exit_s)
            return (0);
        len = len - 1 + get_len(g_exit_status);
        new = var_join(str, exit_s, i, len);
        free(exit_s);
    }
    else
    {
        var_len = (int)ft_strlen(var);
        len = len - 1;
        while (is_al((*str)[i + len - (int)ft_strlen(*str) + 1]))
            len--;
        len += var_len;
        new = var_join(str, var, i, len);
    }
    if (!new)
        return (0);
    free(*str);
    *str = new;
    return (1);
}

int expander(char **str, t_env *env)
{
    int     i;
    int     q_s;
    int     q_d;
    char    *var;

    if (!str || !*str)
        return (0);
    i = 0;
    q_s = 0;
    q_d = 0;
    while ((*str)[i])
    {
        if ((*str)[i] == '\'' && !q_d)
            q_s = !q_s;
        else if ((*str)[i] == '\"' && !q_s)
            q_d = !q_d;
        else if ((*str)[i] == '$' && !q_s)
        {
            i++;
            if (!(*str)[i])
                break ;
            if ((*str)[i] == '?')
                var = NULL;
            else if (!is_al((*str)[i]))
                continue ;
            else
                var = find_var(&(*str)[i], env);
            if (!replace_var(str, var, i))
                return (0);
            continue ;
        }
        i++;
    }
    return (1);
}
