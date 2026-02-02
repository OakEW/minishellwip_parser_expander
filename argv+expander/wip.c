#include <stdlib.h>
#include "lexer.h"

#include<stdio.h>
int	main(int argc, char **argv, char**envp)
{
	char	*env;
	if (argc != 2)
		return (0);

	env = getenv(argv[1]);
	if (env)
		printf("%s\n", env);
	else
		printf("%s\n", argv[1]);
	// while(*envp)
	// {
	// 	printf ("evnp = %s\n", *envp);
	// 	envp++;
	// }
}