NAME= minishell

SRC_DIR= src/
OBJ_DIR= obj/
INC= include/

SRC= tester_ed.c lexer.c lexer_helper.c wildcard.c init_env.c parser.c \
	 parser_helper.c syntax_check.c expander.c expander_helper.c \
	 trimmer.c helper_itoa.c helper_free.c sort_strs.c

OBJ= ${addprefix ${OBJ_DIR}, ${SRC:.c=.o}}

LIB= -lreadline

all: ${NAME}

${NAME}: ${OBJ}
	@echo "[${NAME}] Compiling $@"
	@cc -Wall -Wextra -g -O0 -I${INC} -o $@ $^ ${LIB}

${OBJ_DIR}:
	mkdir -p ${OBJ_DIR}

${OBJ_DIR}%.o: ${SRC_DIR}%.c | ${OBJ_DIR}
	@echo "[${NAME}] Compiling $@"
	@cc -Wall -Wextra -g -O0 -I${INC} -c $< -o $@

clean:
	rm -f ${OBJ}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
