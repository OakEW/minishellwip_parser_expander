SHELL=			zsh
CC=				cc
CFLAGS=			-Wall -Wextra -g -O0 
RM=				rm -f

NAME=			minishell
BIN_DIR=		bin/
NAME:=			${addprefix ${BIN_DIR}, ${NAME}}

SRC=			main.c															\
				cmd.c															\
				tester.c														\
				bt_print.c														\
				exec.c															\
				get_cmd.c														\												\
				error_utils.c													\
				file_utils.c													\
				lexer.c															\
				lexer_helper.c													\
				wildcard.c														\
				init_env.c														\
				parser.c														\
				parser_helper.c													\
				syntax_check.c													\
				expander.c														\
				trimmer.c														\
				helper_itoa.c													\
				helper_free.c

SRC_DIR=		src/

OBJ=			${SRC:.c=.o}
OBJ_DIR=		build/
OBJ:=			${addprefix ${OBJ_DIR}, ${OBJ}}

INC=			include/

LIBFT=			lib/libft.a
LIB_DIR=		lib/

ECHO_PRINT=		[\033[0;35m${NAME}\033[0m]

LIB=			-Llib -lft -lreadline

all:			${NAME} bin/exiter

bin/exiter:
				@echo "${ECHO_PRINT} [\033[0;32mCompiling\033[0m] $@"
				@${CC} ${CFLAGS} -I${INC} -o bin/exiter src/exiter.c ${LIB}

${NAME}:		${BIN_DIR} ${OBJ_DIR} ${LIB_DIR} ${LIBFT} ${OBJ} | ${OBJ_DIR}.
				@echo "${ECHO_PRINT} [\033[0;32mCompiling\033[0m] $@"
				@${CC} ${CFLAGS} -I${INC} -o ${NAME} ${OBJ} ${LIB}

%/:
				@echo "${ECHO_PRINT} [\033[0;32mCreating Directory\033[0m] $@"
				@mkdir -p $@

${LIBFT}:
				@make -s -j 12 -C libft
				@cp libft/bin/libft.a ${LIBFT}

bonus:			all

clean:
				@echo "${ECHO_PRINT} [\033[0;32mRemoving\033[0m] ${OBJ}"
				@${RM} ${OBJ}
				@make -s -j 12 -C libft clean

fclean:
				@echo "${ECHO_PRINT} [\033[0;32mRemoving\033[0m] ${NAME}"
				@echo "${ECHO_PRINT} [\033[0;32mRemoving\033[0m] ${LIBFT}"
				@echo "${ECHO_PRINT} [\033[0;32mRemoving\033[0m] ${OBJ}"
				@${RM} ${NAME}
				@${RM} ${LIBFT}
				@${RM} ${OBJ}
				@make -s -j 12 -C libft fclean

re:				fclean all

%.o:			../${SRC_DIR}%.c
				@echo "${ECHO_PRINT} [\033[0;32mCompiling\033[0m] $@"
				@${CC} ${CFLAGS} -I${INC} -c -o $@ $<

gitadd:			fclean
				git add src include Makefile libft/src libft/include libft/Makefile minilibx-linux

.PHONY:			all bonus clean fclean re gitadd
