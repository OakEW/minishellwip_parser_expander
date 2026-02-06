#include "argv_env.h"

int	op_mix(t_token_type x)
{
	if (x == PIPE || x == THEN || x == ELSE)
		return (1);
	else if (x == INPUT || x == OUTPUT || x == HEREDOC || x == APPEND)
		return (2);
	else if (x == PARENT_O)
		return (3);
	else if (x == PARENT_C)
		return (4);
	else
		return (0);
}

int	syntax_check(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	if (op_mix(token->type) == 1)
		return (258);
	while (tmp)
	{
		if (tmp->next == NULL && op_mix(tmp->type) > 0 && op_mix(tmp->type) < 3)
			return (258);
		if (op_mix(tmp->type) == 1 && tmp->next && op_mix(tmp->next->type) == 1)
			return (258);
		if (op_mix(tmp->type) == 1 && tmp->next && op_mix(tmp->next->type) == 4)
			return (258);
		if (op_mix(tmp->type) == 2 && tmp->next && op_mix(tmp->next->type) > 0)
			return (258);
		if (op_mix(tmp->type) == 3 && tmp->next && op_mix(tmp->next->type) == 1)
			return (258);
		if (op_mix(tmp->type) == 3 && tmp->next && op_mix(tmp->next->type) == 4)
			return (258);
		if (tmp->next && tmp->next->next && op_mix(tmp->type) == 2
				&& op_mix(tmp->next->type) == 0 && op_mix(tmp->next->next->type) >= 3)
			return (258);
		tmp = tmp->next;
	}
	return (0);
}
