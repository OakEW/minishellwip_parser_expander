/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 11:34:26 by ywang2            #+#    #+#             */
/*   Updated: 2026/01/21 12:23:17 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

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
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

int		is_operator(char *str);
void	free_tokens(t_token *head);
int		token_len(char *str, t_token_type type);
char	*token_strndup(const char *s, int n, t_token_type type);
t_token	*new_token(char *str, t_token_type type);
int		make_token(t_token **head, t_token **cur, t_token_type t, char *str);
int		precheck_line(char *line);
t_token	*lexer(char	*line);

#endif