/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_env.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 11:34:26 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/12 12:56:16 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGV_ENV_H
# define ARGV_ENV_H

# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <dirent.h>
# include <errno.h>

# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define MAGENTA	"\033[35m"
# define CYAN		"\033[36m"
# define WHITE		"\033[37m"
# define RESET		"\033[0m"

typedef enum e_token_type
{
	WORD,
	DOUBLE_Q,
	SINGLE_Q,
	PIPE,
	INPUT,
	OUTPUT,
	PARENT_O,
	PARENT_C,
	HEREDOC,
	APPEND,
	THEN,
	ELSE,
	REDIR,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_argv
{
	t_token_type	type;
	char			**argv;
	t_redir			*redir;
	int				argc;
	struct s_argv	*next;
}	t_argv;

typedef struct s_env
{
	char	*last_dir;
	char	**env;
	int		cap;
	int		size;
	int		exit_s;
}	t_env;

typedef struct s_entry
{
	char	**entry;
	int		cap;
	int		match;
}	t_entry;

//init_env.c
size_t	ft_strlen(const char *str);
char	*ft_strdup(char *s);
int		init_env(t_env *env, char **envp);

//lexer_helper.c
int		is_operator(char *str);
int		token_len(char *str);
char	*token_strndup(const char *s, int n);

//lexer.c
t_token	*make_token(char *str, t_token_type type);
int		link_token(t_token **h, t_token **cur, t_token_type t, char *s);
t_token	*lexer(char	*line, t_env *env);

//syntax_check.c
int		precheck_line(char *line);
int		o_p(t_token_type x);
char	*syntax_check(t_token *token);
int		syntax_error(char *line, t_token *token, t_env *env);

//parser_helper.c
t_argv	*argv_init(t_token *token);
int		count_argc(t_token **token, int *flag);
int		make_str(t_argv *argv, t_token *token);

//parser.c
int		make_argv_helper(t_argv **h, t_argv **curt, t_token **t, int *f);
t_argv	*make_argv(t_token *token);
int		build_argv(char *line, t_env *env, t_argv **out);		//lexer->snytax->parser, build t_argv. line and t_token are freed from here
																//return (-1) on syntax error or line is empty 
																//return (0) on malloc fails. return (1) on sucess

//wildcard_entry.c
int		entry_len(t_env *env);
int		get_entry(t_env *env, t_entry *entry);

//wildcard.c
int		join_wild(t_argv *curt, int pos, t_entry *entry);
int		pattern_matching(char *pattern, t_entry *entry);
int		wildcards(t_argv *curt, t_entry *entry, int i);
int		check_wildcard(t_argv *curt, t_env *env);				//expand wildcard *

//expander_helper_var.c
char	*find_var(char *str, t_env *env);
char	*var_join(char **str, char *add, int pos, int len);
char	*replace_var_helper(char **str, int i, t_env *env);
int		replace_var(char **str, char *add, int i, t_env *env);

//expander.c
int		expand_home(t_argv *curt, t_env *env);					//expand ~ to HOME
int		expander_helper(char **str, t_env *env);
int		expander(t_argv *curt, t_env *env);						//expand $ENV
int		expand_all(t_argv *curt, t_env *env);		//do check_wildcard -> expand_home -> rm_char -> expander -> trim_empty -> trim_quote; 
													//return (>0) on malloc fails. return (0) on success

//rm_char.c
int		check_q(char c, int *q_s, int *q_d);
void	int_init(int *i);
void	rm_char_helper(char **str);
void	rm_char(t_argv *curt);									//remove $ when $" || $'

//trimmer.c
void	trim_q(char **s);
void	trim_quote(t_argv *curt);								//remove outer "" || ''
int		rm_empty(t_argv *curt, int i);
int		trim_empty(t_argv *curt);								//trim off empty strstr from t_argv->argv after expand $ENV. 
																//if node becomes empty after expand t_argv->argc = 0 and t_argv->argv = NULL

//helper_itoa.c
int		is_al(char c);
int		get_len(int n);
char	*ft_itoa(int n);

//helper_free.c
void	free_strstr(char **s);
void	free_env(t_env *env);
void	free_tokens(t_token *head);
void	free_argv(t_argv *head);
void	free_entry(t_entry *entry);

//sort_strs.c
int		ft_strcmp(char *s1, char *s2);
void	sort_entry(char **entry);

#endif

/*

...
int	i;
if (*line)
	add_history(line);
	i = build_argv(line, &env, &head);	//init t_argv (do lexer syntax check then parser)
	if (i == -1)						//return (-1) when line is empty || syntax error. line & t_token are freed
		continue ;
	if (i == 1)							//return (1) on success, line & t_token are freed already
		do_something;
...
____________________

int	expand_all(t_argv *curt, t_env *env)
{
	int	i;

	i = 0;
	if (curt->type > 2)
		return (0);
	if (!check_wildcard(curt, env))
		return (1);
	if (!expand_home(curt, env))
		return (2);
	rm_char(curt);
	if (!expander(curt, env))
		return (3);
	if (!trim_empty(curt))
		return (4);
	trim_quote(curt);
	return (0);
}
...
	if(expand_all(tmp, env))											//expander($ ~ *) and trim off outer qoutes
		return (free_argv(head), free_env(env), exit (1), 0); 			//retrun (> 0) on malloc fail, exit (1)
...

*/