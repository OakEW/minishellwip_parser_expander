
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

#include<stdio.h>

typedef enum e_token_type
{
	WORD,		// *
	SINGLE_Q,	// '*'
	DOUBLE_Q,	// "$"
	PIPE,		// |
	INPUT,		// <
	OUTPUT,		// >
	PARENT_O,	// (
	PARENT_C,	// )
	HEREDOC,	// <<
	APPEND,		// >>
	THEN,		// &&
	ELSE,		// ||
}   t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

int	is_special(char *c)
{
	if (*c == '|' || *c == '\'' || *c == '\"' || *c == '<' || *c == '>' || *c == '(' || *c == ')')
		return (1);
	else if (*c == '&' && *(c + 1) == '&')
		return (1);
	return (0);

}

void	free_tokens(t_token *head)
{
	t_token *tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

int	ft_strlen(char *str, int type)
{
	int		x;
	char	q;

	x = 0;
	q = *str;
	if (!str)
		return (0);
	if (type == SINGLE_Q || type == DOUBLE_Q)
	{
		str++;
		while (str[x] && str[x] != q)
			x++;
	}
	else
	{
		while (str[x] > 32 && !is_special(&str[x]))
			x++;
	}
	return (x);
}

int quote_len(char *s)
{
	int		x;
	char	q;

	x = 0;
	q = *s;

	s++;
	while (s[x] && s[x] != q)
		x++;
	return x;
}

char	*ft_strndup(const char *s, int n, int type)
{
	char	*dup;
	int		i;

	if (n <= 0)
		return (NULL);
	i = 0;
	dup = (char *) malloc(sizeof(char) * (n + 1));
	if (dup == 0)
		return (0);
	if (type == SINGLE_Q || type == DOUBLE_Q)
	{
		s++;
		n--;
	}
	while (i < n)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int	is_operator(char *str)
{
	if (*str == '|' && *(str + 1) != '|')
		return (PIPE);
	else if (*str == '(')
		return (PARENT_O);
	else if (*str == ')')
		return (PARENT_C);
	else if (*str == '<' && *(str + 1) != '<')
		return (INPUT);
	else if (*str == '>' && *(str + 1) != '>')
		return (OUTPUT);
	else if (*str == '&' && *(str + 1) == '&')
		return (THEN);
	else if (*str == '|' && *(str + 1) == '|')
		return (ELSE);
	else if (*str == '<' && *(str + 1) == '<')
		return (HEREDOC);
	else if (*str == '>' && *(str + 1) == '>')
		return (APPEND);
	else if (*str == '\'')
		return (SINGLE_Q);
	else if (*str == '\"')
		return (DOUBLE_Q);
	return(0);
}

t_token *new_token(t_token_type type, char *str)
{
	t_token	*token;
	int		len;

	len = 1;
	if (type > 7)
		len = 2;
	else if (type == 0)
		len = ft_strlen(str, type);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (str)
	{
		token->value = ft_strndup(str, len, type);
		if (!token->value)
			return (free(token), NULL);
	}
	token->next = NULL;
	return (token);
}

int	add_token(t_token **head, t_token **current, t_token_type type, char *str)
{
	t_token *token;
	
	token = new_token(type, str);
	if (!token)
		return (-1);
	if (!*head)
		*head = token;
	else
		(*current)->next = token;
	*current = token;
	if (type >= 3 && type <= 7)
		return (1);
	else if (type > 7)
		return (2);
	return (ft_strlen(str, type));
}

t_token	*lexer(char	*line)
{
	int		i;
	int		n;
	t_token	*current;
	t_token *head;
	int		type;

	current = NULL;
	head = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && line[i] <= 32)
			i++;
		if (line[i] == 0)
			break ;
		type = is_operator(&line[i]);
		n = add_token(&head, &current, type, &line[i]);
		if (n < 0)
			return (free_tokens(head), NULL);
		i += n;
	}
	return (head);
}





// for testing

void print_tokens(t_token *head)
{
	t_token *tmp;
	
	tmp = head;
	while (tmp)
	{
		printf("Token type: %d, value: '%s'\n", tmp->type, tmp->value);
		tmp = tmp->next;
	}
}


int main(int ac, char **line)
{

	if (ac != 2)
		return (0);
	t_token *head;

	head = lexer(line[1]);
	print_tokens(head);
	free_tokens(head);

	return 0;
}
