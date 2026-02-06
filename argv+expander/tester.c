#include "argv_env.h"


// // for test


int	print_argv(t_argv *head, t_env *env)
{
	t_argv *tmp;

	int	x = 0;
	tmp = head;
	while (tmp)
	{
		int i = 0;
		while (i < tmp->argc)
		{
			trim_expand(tmp, env);
			printf(BLUE"t_argv[%d] type:%d",x, tmp->type);
			printf(GREEN" -->argv[%d]: {%s}\n"RESET, i, tmp->argv[i]);
			i++;
		}
		printf ("\n");
		x++;
		tmp = tmp->next;
	}
	return (0);
}

void handle_sigint(int sig)	// ctrl -C
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
	t_argv *head;
	t_env	*env;

	env = init_env(envp);
	if (!env)
		return (0);
	signal(SIGQUIT, SIG_IGN);		//"ctrl -\"
	signal(SIGINT, handle_sigint);	// ctrl -C
	while (1)
	{
		line = readline(GREEN"M_S->"RESET);
		if (!line)
			break ;
		if (*line)
			add_history(line);
		if(build_argv(line, env, &head))
		{
			print_argv(head, env);
			env->exit_s = 0;
			free_argv(head);						// free struct argv in main
		}
		free(line);
	}
	rl_clear_history();
	free_env(env);
	write(1, YELLOW"Exit Mini_Shell\n"RESET, 25);
	// signal(SIGQUIT, SIG_DFL);
	return (0);
}
