/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_env.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 11:34:26 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/06 17:40:36 by ywang2           ###   ########.fr       */
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

# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN    "\033[36m"
# define WHITE   "\033[37m"
# define RESET   "\033[0m"

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
	char	**env;
	int		cap;
	int		size;
	int		exit_s;
}	t_env;

//lexer.c
int		is_operator(char *str);
int		token_len(char *str, t_token_type type);
char	*token_strndup(const char *s, int n);

//lexer_helper.c
t_token	*make_token(char *str, t_token_type type);
int		link_token(t_token **h, t_token **cur, t_token_type t, char *s);
int		precheck_line(char *line);
t_token	*lexer(char	*line);

//init_env.c
size_t	ft_strlen(const char *str);
char	*ft_strdup(char *s);
t_env 	*init_env(char **envp);

//syntax_check.c
int		o_p(t_token_type x);
int		syntax_check(t_token *token);
int		syntax_error(char *line, t_token *token, t_env *env);

//parser_helper.c
t_argv	*argv_init(t_token *token);
int		count_argc(t_token **token, int *flag);
int		make_str(t_argv *argv, t_token *token);

//parser.c
int		make_argv_helper(t_argv **h, t_argv **curt, t_token **t, int *f);
t_argv	*make_argv(t_token *token, t_env*env);
int		build_argv(char *line, t_env *env, t_argv **out);

//wildcard.c
int		entry_len(t_env *env);
char	**get_entry(t_env *env);
char	**join_wild_helper(t_argv *curt, char **entry, t_env *env);
int		join_wild(t_argv *curt, int pos, char **entry, t_env *env);
int		wildcards(t_argv *curt, t_env *env);

//helper_itoa.c
int		is_al(char c);
int		get_len(int n);
char	*ft_itoa(int n);

//expander.c
char	*find_var(char *str, t_env* env);
char	*var_join(char **str, char *add, int pos, int len);
int		replace_var(char **str, char *add, int i, t_env *env);
int		expander(char **str, t_env* env);

//trimmer.c
int		rm_empty(t_argv *curt, int i);
void	trim_q(char *s);
int		trim_expand(t_argv *curt, t_env *env);

//helper_free.c
void	free_strstr(char **s);
void	free_env(t_env *env);
void	free_tokens(t_token *head);
void	free_argv(t_argv *head);
#endif
