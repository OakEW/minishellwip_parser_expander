/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_ed.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:33:14 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/10 11:49:50 by ywang2           ###   ########.fr       */
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
		if(!trim_expand(tmp, env))	//expander($ and *) and trim off outer qoutes
			return (free_argv(head), free_env(env), malloc_fail(env), 0); // malloc fail, msg written, exit (1)
		while (i < tmp->argc)
		{
			printf(BLUE "t_argv[%d] type:%d", x, tmp->type);
			printf(GREEN " -->argv[%d]: {%s}\n" RESET, i, tmp->argv[i]);
			i++;
		}
		printf ("\n");
		x++;
		tmp = tmp->next;
	}
	return (1);
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
	t_env	env;
	int		i;

	(void)argc;
	(void)argv;
	if (!init_env(&env, envp))
		return (1);
	signal(SIGQUIT, SIG_IGN);				// ctrl -/
	signal(SIGINT, handle_sigint);			// ctrl -C
	while (1)
	{
		line = readline(GREEN"M_S->"RESET);
		if (!line)
			break ;
		if (*line)
			add_history(line);
		i = build_argv(line, &env, &head);	//init t_argv (do lexer then parser + syntax check)
		if (i == -1)						//return (-1) when line is empty, or syntax error . line & t_token are freed. msg written, exit (1)
			continue ;
		if (i == 1)							//return (1) on success, line & t_token are freed
		{
			if (print_argv(head, &env))		// expand and trim qoutes in "child"
			{
				(&env)->exit_s = 0;
				free_argv(head);
			}
		}								//return (0) if syntax error, t_token and t_argv are freed. msg written, exit (1)
	}
	free_env(&env);
	rl_clear_history();
	write(1, YELLOW"Exit Mini_Shell\n"RESET, 25);
	return (0);
}
// signal(SIGQUIT, SIG_DFL);
