/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_ed.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:33:14 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/07 16:00:10 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argv_env.h"

int	print_argv(t_argv *head, t_env *env)
{
	t_argv	*tmp;
	int		x;
	int		i;

	x = 0;
	tmp = head;
	while (tmp)
	{
		i = 0;
		while (i < tmp->argc)
		{
			trim_expand(tmp, env);
			printf(BLUE "t_argv[%d] type:%d", x, tmp->type);
			printf(GREEN " -->argv[%d]: {%s}\n" RESET, i, tmp->argv[i]);
			i++;
		}
		printf ("\n");
		x++;
		tmp = tmp->next;
	}
	return (0);
}

// ctrl -C
void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_argv	*head;
	t_env	*env;
	int		i;

	env = init_env(envp);
	if (!env)
		return (0);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sigint);
	while (1)
	{
		line = readline(GREEN"M_S->"RESET);
		if (!line)
			break ;
		if (*line)
			add_history(line);
		i = build_argv(line, env, &head);
		if (i == -1)
			continue ;
		if (i == 1)
		{
			print_argv(head, env);
			env->exit_s = 0;
			free_argv(head);
		}
		free(line);
	}
	rl_clear_history();
	free_env(env);
	write(1, YELLOW"Exit Mini_Shell\n"RESET, 25);
	return (0);
}
// signal(SIGQUIT, SIG_DFL);
