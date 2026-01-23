#include "ft_printf.h"
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

// #include<stdio.h>

int	is_space(char *str, int i)
{
	int	checker;
	int	j;

	checker = 0;
	j = i;
	while(str[j])
		checker += (str[j++] == '/');
	j = i;
	while(str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
		i++;
	if (i == j && checker != 0)
		return (write(2, "No such file or directory\n", 27), -1);
	else if (i == j && checker == 0)
		return (write(2, "command not found\n", 19), -1);
	return (i);
}

int	ft_strncmp(char *s1, char *s2, int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] && s2[i] && (i < n - 1) && (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]);
}

int	echo_flag(char *str)
{
	int	i;
	int	x;

	i = 0;
	x = 1;
	if (str[i] != '-')
		return (0);
	i++;
	while(str[i] && (str[i] != ' ' && str[i] != '\t' && str[i] != '\n'))
	{
		if (str[i] != 'n')
			x = -1;
		i++;
	}
	while(str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
		i++;
	if (x < 0)
		return (0);
	return (i);
}

int	ft_strlen(char *str)
{
	int	x;

	x = 0;
	if (!str)
		return (0);
	while (str[x] != '\0')
		x++;
	return (x);
}


int	my_echo(char *line)
{
	int	skip;
	int	flag;
	int	len;

	len = ft_strlen(line);
	signal(SIGQUIT, SIG_DFL);
	if (line[4] == 0)
		return (write(1,"\n",1), 0);
	skip = is_space(line, 4);
	if (skip < 0)
		return (127);
	flag = echo_flag(&line[skip]);
	while (flag + skip < len)
	{
		write(1, &line[flag + skip], 1);
		skip++;
	}
	if (flag == 0)
		write(1, "\n", 1);
	return (0);
}

int	my_cd(char *line)
{
	int		i;
	int		skip;
	char	*pwd;

	signal(SIGQUIT, SIG_DFL);
	if (line[2] == 0)
		return (0);
	skip = is_space(line, 2);
	if (skip < 0)
		return (127);
	i = chdir(&line[skip]);
	if (i == -1)
		return (write(2, "22 such file or directory\n", 27), -1);
	return (0);
}

int my_pwd(void)
{
	char *pwd;

	signal(SIGQUIT, SIG_DFL);
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (write(2, "No such file or directory\n", 27), -1);
	ft_printf("%s\n", pwd);
	free(pwd);
	return (0);
}

int	my_exit(void)
{
	write(1, "Exit Mini_Shell\n", 17);
	exit (0);
}



void handle_sigint(int sig)	// ctrl -C
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	do_buildin(char *line)
{
	while(*line && (*line == ' ' || *line == '\t' || *line == '\n'))
		line++;
	if (*line)
		add_history(line);
	if (ft_strncmp(line, "pwd", 3) == 0)
		my_pwd();
	if (ft_strncmp(line, "cd", 2) == 0)
		my_cd(line);
	if (ft_strncmp(line, "echo", 4) == 0)
		my_echo(line);
	if (ft_strncmp(line, "exit", 4) == 0)
		my_exit();
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	signal(SIGQUIT, SIG_IGN);		//"ctrl -\"
	signal(SIGINT, handle_sigint);	// ctrl -C
	if (argc != 1)
		exit (1);
	while (1)
	{
		line = readline("M-S $ ");
		if (!line)
		break ;
		do_buildin(line);
		free(line);
	}
	rl_clear_history();
	write(1, "Exit Mini_Shell\n", 17);
	return (0);
}

	// signal(SIGQUIT, SIG_DFL);
