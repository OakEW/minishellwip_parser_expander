/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 11:34:26 by ywang2            #+#    #+#             */
/*   Updated: 2026/02/03 18:24:30 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

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
}	t_env;

int		is_operator(char *str);
void	free_tokens(t_token *head);
int		token_len(char *str, t_token_type type);
char	*token_strndup(const char *s, int n);
t_token	*make_token(char *str, t_token_type type);
int		link_token(t_token **head, t_token **cur, t_token_type t, char *str);
int		precheck_line(char *line);
t_token	*lexer(char	*line);
void	free_argv(t_argv *head);

char	*ft_strdup(char *s);
t_env	*init_env(char **envp);
int		expander(char **str, t_env *env);
size_t	ft_strlen(const char *str);
#endif
